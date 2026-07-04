/**
 * @file       LLMProcessing.h
 * @brief      Declaration of the LLMProcessing class for natural language processing and emotion analysis.
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
#ifndef LLMPROCESSING_H
#define LLMPROCESSING_H

#include <string>
#include <list>

/**
 * @class LLMProcessing
 * @brief Class for natural language processing and emotion analysis.
 */
class LLMProcessing {
private:
    std::list<std::string> conversationContext; ///< List to store conversation context.
    std::string emotionState; ///< String to store the emotion state.

public:
    /**
     * @brief Default constructor for LLMProcessing class.
     */
    LLMProcessing() = default;

    /**
     * @brief Default destructor for LLMProcessing class.
     */
    ~LLMProcessing() = default;
    
    /**
     * @brief Generates a response based on the provided text input.
     * @param[in] textInput The input text for response generation.
     * @return The generated response.
     */
    std::string generateResponse(const std::string& textInput);

    /**
     * @brief Determines the emotion associated with the provided response.
     * @param[in] response The response text for emotion analysis.
     * @return The determined emotion label.
     */
    std::string determineEmotion(const std::string& response);
};

/**
 * @brief Executes a system command and returns the output.
 * @param[in] cmd The command to be executed.
 * @return The output of the executed command.
 */
std::string exec(const char* cmd);

/**
 * @brief Extracts the emotion label from the provided JSON response.
 * @param[in] json The JSON response containing emotion information.
 * @return The extracted emotion label.
 */
std::string extractLabel(const std::string& json);

#endif // LLMPROCESSING_H
