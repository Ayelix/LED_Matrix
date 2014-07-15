/* json-utils - Utility class with helper methods that handle JSON reading and
 * writing.  Based on the jsoncpp library.
 */
#ifndef _MATRIX_JSON_UTILS_H
#define _MATRIX_JSON_UTILS_H

#include <string>

#include <led-matrix/controller/controller.hpp>
#include <led-matrix/controller/mode/mode.hpp>

namespace JSONUtils
{
    // Get a JSON string containing the following members from the given
    // MatrixController:
    // numModes : <number of available modes for the controller>
    // modes : <array of modes for the controller>
    // currentMode : <index of the controller's current mode>
    std::string getModes(MatrixController const * controller);
    
    // Get a JSON string containing the following members from the given
    // MatrixMode and mode index:
    // mode : <mode object for the given mode index>
    // numSettings : <number of settings for the mode>
    // settings : <array of settings for the mode>
    std::string getSettings(MatrixMode const * mode, unsigned int modeIndex);
};

#endif