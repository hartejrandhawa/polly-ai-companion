//SAAD MAHMOOD
#include "../include/polly/LLMProcessing.h"
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string exec(const char* cmd);
std::string cleanResponse(const std::string& resp, const std::string& json);
std::string extractLabel(const std::string& json);

std::string LLMProcessing::generateResponse(const std::string& textInput) {
    //escaping for JSON
    std::string escapedTextInput;
    for (char c : textInput) {
        if (c == '\"') {
            escapedTextInput += "\\\"";
        } else {
            escapedTextInput += c;
        }
    }
    std::string command = "curl -s -X POST -H \"Content-Type: application/json\" -d \"{\\\"text\\\": \\\"" + escapedTextInput + "\\\"}\" http://localhost:5000/generate > temp.txt";
    system(command.c_str());

    //Emulate context saving
    //conversationContext.push_back(response);
    std::ifstream ifs("temp.txt");
    std::string response((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();
    std::remove("temp.txt"); //garbage coll
    response = cleanResponse(response, escapedTextInput);
    return response;
}

std::string LLMProcessing::determineEmotion(const std::string& response) {
    //escaping for JSON
    //std::string escapedResponse;
    //for (char c : response) {
    //    if (c == '\"') {
    //        escapedResponse += "\\\"";
    //    } else {
    //        escapedResponse += c;
    //    }
    //}


    //std::string jsonPayload = "{\"text\": \"" + response + "\"}";
    //std::string command = "curl -X POST -H \"Content-Type: application/json\" -d " + jsonPayload + " http://localhost:5000/generate_emotion > temp2.txt";
    std::string halfResponse;

    // Calculate the length of the first half
    int halfLength = response.length() / 3;

    // Create the substring from the beginning to the halfway point
    halfResponse = response.substr(0, halfLength);

    std::string command = "curl -s -X POST -H \"Content-Type: application/json\" -d \"{\\\"text\\\": \\\"" + halfResponse + "\\\"}\" http://localhost:5000/generate_emotion > temp2.txt";

    //std::string command = "curl -X POST -H \"Content-Type: application/json\" -d " + jsonPayload + " http://localhost:5000/generate_emotion > temp2.txt";


    //std::cout << "command";
    //std::cout << command;
    //std::string command = "curl -X POST -H \"Content-Type: application/json\" -d \"{\\\"text\\\": \\\"" + escapedResponse + "\\\"}\" http://localhost:5000/generate_emotion > temp.txt";
    system(command.c_str());
    //conversationContext.push_back(response);
    std::ifstream ifs("temp2.txt");
    std::string emotion((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();
    std::remove("temp2.txt");

    //std::cout << "EMOTION IS::";
    //std::cout << emotion;
    //std::cout << ":: WAS THE EMOTION";
    return extractLabel(emotion);
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string cleanResponse(const std::string& resp, const std::string& originalPrompt) {
    std::string modifiedResp = resp;  // Make a copy of the input string

    // Find the position of "result":
    size_t resultPos = modifiedResp.find("\"result\":");

    // If "result" is found, remove it and the surrounding characters
    if (resultPos != std::string::npos) {
        modifiedResp.erase(0, resultPos + 10); // 10 is the length of "\"result\":"
    }

    //Find and remove the original prompt from the response  **MIGHT BE BETTER TO TUNE THE LLM**
    //size_t promptPos = modifiedResp.find(originalPrompt);

    //modifiedResp.erase(0, originalPrompt.length());


    std::string cleanResp = "";
    for (size_t i = 0; i < modifiedResp.length(); ++i) {
        char currentChar = modifiedResp[i];

        if (currentChar == '{' || currentChar == '}' || currentChar == '"' || currentChar == '/' || currentChar == '\\') {
            // Check if new line symbol
            if (i + 1 < modifiedResp.length() && modifiedResp[i + 1] == 'n') {
                // Skip the '\n' character
                ++i;
                continue;
            }
            continue;
        }

        // Append the character to cleanResp
        cleanResp += currentChar;
    }

    return cleanResp;
}


std::string extractLabel(const std::string& json) {
    std::size_t labelPos = json.find("\"label\": ");
    if (labelPos == std::string::npos) {
        return "Label not found";
    }

    std::size_t start = json.find("\"", labelPos + 8);
    std::size_t end = json.find("\"", start + 1);

    if (start != std::string::npos && end != std::string::npos) {
        return json.substr(start + 1, end - start - 1);
    }

    return "Label not found";
}

/* Using the following dataset for sentiment analysis:
 * huggingface.co/datasets/dair-ai/emotio
 *
 * and the following model (trained on that dataset) :: https://huggingface.co/esuriddick/distilbert-base-uncased-finetuned-emotion
 *
 * CITATION (as per request of dataset creators):
 * @inproceedings{saravia-etal-2018-carer,
    title = "{CARER}: Contextualized Affect Representations for Emotion Recognition",
    author = "Saravia, Elvis  and
      Liu, Hsien-Chi Toby  and
      Huang, Yen-Hao  and
      Wu, Junlin  and
      Chen, Yi-Shin",
    booktitle = "Proceedings of the 2018 Conference on Empirical Methods in Natural Language Processing",
    month = oct # "-" # nov,
    year = "2018",
    address = "Brussels, Belgium",
    publisher = "Association for Computational Linguistics",
    url = "https://www.aclweb.org/anthology/D18-1404",
    doi = "10.18653/v1/D18-1404",
    pages = "3687--3697",
    abstract = "Emotions are expressed in nuanced ways, which varies by collective or individual experiences, knowledge, and beliefs. Therefore, to understand emotion, as conveyed through text, a robust mechanism capable of capturing and modeling different linguistic nuances and phenomena is needed. We propose a semi-supervised, graph-based algorithm to produce rich structural descriptors which serve as the building blocks for constructing contextualized affect representations from text. The pattern-based representations are further enriched with word embeddings and evaluated through several emotion recognition tasks. Our experimental results demonstrate that the proposed method outperforms state-of-the-art techniques on emotion recognition tasks.",
}
 */

