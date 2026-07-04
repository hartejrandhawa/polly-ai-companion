/**
 * @file       Display.h
 * @brief      Declaration of the Display class for handling display-related functionalities.
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
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

namespace tinygltf {
    class Model; //forward dec
}

/**
 * @class Display
 * @brief Class for handling display-related functionalities.
 */
class Display {
public:
    /**
     * @brief Sends the specified emotion to the connected WebSocket.
     * @param[in] emotion The emotion to be sent.
     * @retval None
     */
    void pollyFeels(const std::string& emotion);

    /**
     * @brief Loads a GLTF model from the specified filename.
     * @param[in] filename The name of the file containing the GLTF model.
     * @retval None
     */
    void loadGLTFModel(const std::string& filename);

    /**
     * @brief Parses the specified GLTF model.
     * @param[in] model The GLTF model to be parsed.
     * @retval None
     */
    void parseGLTFModel(const tinygltf::Model& model);

    /**
     * @brief Initiates the display of Polly waves.
     * @retval None
     */
    void pollyWaves();
};

#endif // DISPLAY_H
