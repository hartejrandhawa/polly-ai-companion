// SAAD MAHMOOD

#include "../include/polly/Polly.h"
#include "crow.h"
#include <algorithm>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>

// Declare these as extern in Polly.cpp to access them from main.cpp
extern std::mutex queueMutex;
extern std::queue<std::string> messageQueue;
extern crow::websocket::connection *current_ws_conn;

Polly::Polly() : voice(&tts) { tts.setVoiceCustomization(&voice); }

void Polly::run() {
  tts.speak("STARTING UP"); // Startup message
  tts.speak("Welcome to Polly. How can I assist you today?");

  while (true) {
    std::string textInput = getNextMessageFromWebSocket();
    if (textInput.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    display.pollyFeels("thinking"); // Polly starts to think
    std::transform(textInput.begin(), textInput.end(), textInput.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (textInput == "change voice") {
      voice.changeVoice();
      tts.speak("Voice has been changed.");
    } else if (textInput == "shutdown") {
      tts.speak("Shutting down. Goodbye!");
      break;
    } else {
      std::string response = llmProcessor.generateResponse(textInput);
      std::string emotion = llmProcessor.determineEmotion(response);

      if (current_ws_conn) {
        crow::json::wvalue msg;
        msg["response"] = response;
        msg["emotion"] = emotion;
        current_ws_conn->send_text(crow::json::dump(msg));
      }

      display.pollyFeels(emotion);
      tts.speak(response);
    }
  }
}

std::string Polly::getNextMessageFromWebSocket() {
  std::lock_guard<std::mutex> lock(queueMutex);
  if (!messageQueue.empty()) {
    std::string message = messageQueue.front();
    messageQueue.pop();
    std::cout << "Processing msg: " << message << std::endl; // debugging
    return message;
  }
  return ""; // Return an empty string if no message is available
}
void Polly::narrate(const std::string &response) { tts.speak(response); }
