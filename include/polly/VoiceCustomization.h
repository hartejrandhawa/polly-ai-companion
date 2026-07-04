/**
 * @file       VoiceCustomization.h
 * @brief      Declaration of the VoiceCustomization class for customizing the voice in text-to-speech.
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
#ifndef VOICE_CUSTOMIZATION_H
#define VOICE_CUSTOMIZATION_H

#include <string>

// Forward declaration
class TextToSpeech;

/**
 * @class VoiceCustomization
 * @brief Class for customizing the voice used in text-to-speech.
 */
class VoiceCustomization {
public:
    /**
     * @brief Constructor to initialize VoiceCustomization with a TextToSpeech object.
     * @param[in] tts A pointer to the TextToSpeech object.
     */
    VoiceCustomization(TextToSpeech* tts);

    /**
     * @brief Change the voice by providing options and processing user input.
     */
    void changeVoice();

    /**
     * @brief Get the selected voice.
     * @return A string representing the selected voice.
     */
    std::string getVoice();

private:
    TextToSpeech* tts; ///< Pointer to the TextToSpeech object for voice customization.
};

#endif // VOICE_CUSTOMIZATION_H
