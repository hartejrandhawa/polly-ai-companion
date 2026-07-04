/**
 * @file       STT.h
 * @brief      Declaration of the Speech-to-Text (STT) class for recording audio and performing speech recognition.
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
#ifndef STT_H_
#define STT_H_

#include <vector>
#include <string>
#include "portaudio.h"
#include <curl/curl.h>
#include "nlohmann/json.hpp"

// Forward declaration of SpeechRecognizer
class SpeechRecognizer;

/**
 * @class STT
 * @brief Class for recording audio and performing speech recognition (Speech-to-Text).
 */
class STT {
public:
    /**
     * @brief List available audio devices and print to console.
     */
    void ListAudioDevices();

    /**
     * @brief Record audio from the default input device for a specified duration.
     * @param[in] recordTimeSecs The duration (in seconds) to record audio.
     * @return A vector containing the recorded audio samples.
     */
    std::vector<int16_t> RecordAudio(int recordTimeSecs);

    /**
     * @brief Run the Speech-to-Text process, including audio recording and speech recognition.
     * @return The transcribed text from the recorded audio.
     */
    std::string run();

    // Constants
    static const int kSampleRate; ///< The sample rate for audio recording.
    static const int kFramesPerBuffer; ///< The number of frames per buffer for audio recording.
    static const int kNumChannels; ///< The number of audio channels for recording.
    static const int kRecordTimeSecs; ///< The default duration (in seconds) for audio recording.
};

// Global constant for API key
extern const std::string apikey;

#endif // STT_H_
