#include "../include/polly/VoiceCustomization.h"
#include "../include/polly/TextToSpeech.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

int voiceNum = 0;
VoiceCustomization::VoiceCustomization(TextToSpeech* tts) : tts(tts) {}

void VoiceCustomization::changeVoice() {
    // Define the espeak commands as separate strings
    std::string response1 = "Just say the number associated with the voice you would like to hear";
    tts->speak(response1);
    voiceNum = 0;
    std::string response2 = "This is voice one";
    tts->speak(response2);
    voiceNum=1;
    std::string response3 = "This is voice two";
    tts->speak(response3);
    voiceNum = 0;

    int i = 0;
    while (i == 0) {
        // Call speech to Text
        //std::string userResponse = speechToText(); //Change this
        std::string userResponse = "Two";

        if (userResponse == "One") {
            voiceNum = 0;
            break;
        } else if (userResponse == "Two") {
            voiceNum = 1;
            break;
        } else {
            std::string invalid = "espeak -v en \" Invalid response, try again \"";
            tts->speak(invalid);
        }
    }
}
std::string VoiceCustomization::getVoice() {
    if (voiceNum == 0){
        return "en";
    } else if (voiceNum == 1){
        return "en+f1";
    } else {
        return "en";
    }
}

