#include <led-matrix/webserver/json-utils.hpp>

#include <vector>

#include <json/json.h>

namespace JSONUtils
{
    // Anonymous namespace for "private" items
    namespace
    {
        Json::Value getModeJSON(MatrixMode const * const mode,
            unsigned int index)
        {
            Json::Value retVal;
            retVal["name"] = mode->getName();
            retVal["description"] = mode->getDescription();
            retVal["index"] = index;
            return retVal;
        }
    }
    
    std::string getModes(MatrixController const * const controller)
    {
        // Root value for creating the JSON string
        Json::Value root;
        
        // Get a reference to the mode list from the controller
        std::vector<MatrixMode *> const & modeList = controller->getModes();
        
        // Get the number of modes in the mode list
        root["numModes"] = modeList.size();
        
        // Get the mode object for each mode in the list
        for (unsigned int i = 0; i < modeList.size(); i++)
        {
            root["modes"][i] = getModeJSON(modeList.at(i), i);
        }
        
        // Get the index of the current mode from the controller
        root["currentMode"] = controller->getModeIndex();
        
        // Write the JSON to a string
        Json::StyledWriter writer;
        return writer.write(root);
    }
}