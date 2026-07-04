/**
 * @file       Polly.h
 * @brief      Declaration of the Polly class for an interactive voice assistant.
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
#ifndef POLLY_H
#define POLLY_H

#include "STT.h"
#include "LLMProcessing.h"
#include "TextToSpeech.h"
#include "Display.h"
#include "VoiceCustomization.h"

// Forward declaration of STT
class STT;

/**
 * @class Polly
 * @brief Class representing an interactive voice assistant.
 */
class Polly {
private:
    LLMProcessing llmProcessor; ///< LLMProcessing instance for natural language processing.
    TextToSpeech tts; ///< TextToSpeech instance for speech synthesis.
    Display display; ///< Display instance for handling display-related functionalities.
    VoiceCustomization voice; ///< VoiceCustomization instance for voice customization.
    STT stt; ///< Speech-to-Text instance for converting speech to text.

public:
    /**
     * @brief Default constructor for the Polly class.
     */
    Polly();

    /**
     * @brief Function to run the interactive voice assistant.
     * @return None
     */
    void run();

    /**
     * @brief Retrieves the next message from the WebSocket message queue.
     * @return The next message from the queue.
     */
    std::string getNextMessageFromWebSocket();

    /**
     * @brief Narrates the provided response using text-to-speech.
     * @param[in] response The response to be narrated.
     * @return None
     */
    void narrate(const std::string& response);
};

#endif // POLLY_H
