/**
 * @file       TextToSpeech.h
 * @brief      Declaration of the TextToSpeech class for converting text to speech using espeak.
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Nov-28-2023     Saad Mahmood, Nishanth Pirakalathan, Hartej Singh Randhawa, Taimur Sikander, Paramvir Sransmahmo43@uwo.ca, npirakal@uwo.ca, hrandh9@uwo.ca, tsikande@uwo.ca, psran2@uwo.ca
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 */
#ifndef TEXT_TO_SPEECH_H
#define TEXT_TO_SPEECH_H

#include <string>

// Forward declaration
class VoiceCustomization;

/**
 * @class TextToSpeech
 * @brief Class for converting text to speech using the espeak library.
 */
class TextToSpeech {
public:
    /**
     * @brief Convert the given text to speech.
     * @param[in] text The text to be converted to speech.
     */
    void speak(const std::string& text);

    /**
     * @brief Set the voice customization for text-to-speech.
     * @param[in] voice A pointer to the VoiceCustomization object.
     */
    void setVoiceCustomization(VoiceCustomization* voice);

private:
    VoiceCustomization* voice; ///< Pointer to the VoiceCustomization object for voice customization.
};

#endif // TEXT_TO_SPEECH_H
