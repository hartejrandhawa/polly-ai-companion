// TextToSpeech.cpp

#include "../include/polly/TextToSpeech.h"
#include "../include/polly/VoiceCustomization.h"
#include <cstdlib>
#include <stdexcept>

void TextToSpeech::speak(const std::string& text) {

    std::string voiceVariant = voice->getVoice();
    std::string command = "espeak -v" + voiceVariant + " \"" + text + "\"";

    //std::string command = "espeak -v en+f1 \" Testing voice change \"";
    std::system(command.c_str());
}
void TextToSpeech::setVoiceCustomization(VoiceCustomization* voice) {
    this->voice = voice;
}

