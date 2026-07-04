/**
 * @file       SpeechRecognizer.h
 * @brief      Declaration of the SpeechRecognizer class for performing speech recognition using Google Cloud Speech API.
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
#ifndef SPEECHRECOGNIZER_H
#define SPEECHRECOGNIZER_H

#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/**
 * @class SpeechRecognizer
 * @brief Class for performing speech recognition using Google Cloud Speech API.
 */
class SpeechRecognizer {
public:
    /**
     * @brief Constructor to initialize SpeechRecognizer with an API key.
     * @param[in] apiKey The API key for the speech service.
     */
    SpeechRecognizer(const std::string &apiKey);

    /**
     * @brief Perform speech recognition on an audio buffer.
     * @param[in] audioBuffer The audio buffer to perform speech recognition on.
     * @return The transcribed text from the speech recognition.
     */
    std::string performSpeechRecognition(std::vector<int16_t> audioBuffer);

    /**
     * @brief Extract transcribed text from JSON response.
     * @param[in] jsonResponse The JSON response from the speech recognition service.
     * @return The transcribed text.
     */
    std::string getTranscribedText(const std::string &jsonResponse);

private:
    std::string apiKey; ///< API key for the speech service.

    /**
     * @brief Callback for writing data received from CURL.
     * @param[in] contents Pointer to the received data.
     * @param[in] size Size of each element.
     * @param[in] nmemb Number of elements.
     * @param[out] s String to store the received data.
     * @return The size of the received data.
     */
    static size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);

    /**
     * @brief Encode audio buffer to Base64.
     * @param[in] buffer The audio buffer to be encoded.
     * @return The Base64 encoded audio.
     */
    std::string base64Encode(const std::vector<int16_t> &buffer);
};

#endif //SPEECHRECOGNIZER_H
