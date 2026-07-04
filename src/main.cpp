#include "crow.h"
#include "../include/polly/Polly.h"
#include "../include/polly/LLMProcessing.h"
#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include <thread>

std::mutex mtx;
std::mutex queueMutex;
std::queue<std::string> messageQueue;
LLMProcessing llmProcessor;
Polly pollyPickles;
crow::websocket::connection* current_ws_conn = nullptr;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]() {
        auto page = crow::mustache::load("./webui/index.html").render();
        crow::response res(page);
        res.add_header("Content-Type", "text/html");
        return res;
    });

    CROW_ROUTE(app, "/ws").websocket()
        .onopen([&](crow::websocket::connection& conn) {
            std::lock_guard<std::mutex> lock(mtx);
            current_ws_conn = &conn;
            std::cout << "New WebSocket connection established" << std::endl;
        })
        .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
            if (is_binary) {
                std::cout << "Binary data received (unsupported)" << std::endl;
                return;
            }
            std::lock_guard<std::mutex> lock(queueMutex);
            messageQueue.push(data);
        })
        .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
            std::lock_guard<std::mutex> lock(mtx);
            if (current_ws_conn == &conn) {
                current_ws_conn = nullptr;
            }
            std::cout << "WebSocket connection closed: " << reason << std::endl;
        });

    std::thread([&](){
        while(true) {
            std::string message;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if(!messageQueue.empty()) {
                    message = messageQueue.front();
                    messageQueue.pop();
                }
            }
            if(!message.empty()) {
                //begin thinking
                if (current_ws_conn) {
                    crow::json::wvalue thinkingMsg;
                    thinkingMsg["action"] = "thinking";
                    thinkingMsg["thinkingText"] = " * ｔｈｉｎｋｉｎｇ * ";
                    current_ws_conn->send_text(crow::json::dump(thinkingMsg));
                }
                //get the response and emotion
                std::string response = llmProcessor.generateResponse(message);
                std::string emotion = llmProcessor.determineEmotion(response);

                // Send the response back through the WebSocket
                if (current_ws_conn) {
                    crow::json::wvalue msg;
                    msg["response"] = response;
                    msg["emotion"] = emotion;
                    current_ws_conn->send_text(crow::json::dump(msg));
                    pollyPickles.narrate(response);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).detach();

    app.port(8080).multithreaded().run();
}
