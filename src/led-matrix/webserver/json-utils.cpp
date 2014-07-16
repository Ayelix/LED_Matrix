#include <led-matrix/webserver/json-utils.hpp>

#include <vector>
#include <sstream>

#include <json/json.h>

#include <led-matrix/controller/mode/setting/setting.hpp>

// Setting implementations
#include <led-matrix/controller/mode/setting/setting-string.hpp>
#include <led-matrix/controller/mode/setting/setting-ranged-double.hpp>

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
        
        Json::Value getSettingJSON(MatrixSetting const * const setting,
            unsigned int index)
        {
            Json::Value retVal;
            
            // Get the setting's name and description
            retVal["name"] = setting->getName();
            retVal["description"] = setting->getDescription();
            
            // Get the name for the settings' value type
            std::string type = setting->getType();
            retVal["type"] = type;
            
            // Get the setting's actual value
            Json::Value value;
            switch (setting->getID())
            {
                case MatrixSetting::MATRIX_SETTING_ID_STRING:
                {
                    MatrixSettingString * const settingString =
                        (MatrixSettingString *)setting;
                    value = settingString->getString();
                    break;
                }
                case MatrixSetting::MATRIX_SETTING_ID_RANGED_DOUBLE:
                {
                    MatrixSettingRangedDouble * const settingRangedDouble =
                        (MatrixSettingRangedDouble *)setting;
                    value = settingRangedDouble->getValue();
                    break;
                }
                
                // No default case to preserve compiler warnings for unhandled enum
                // values
                case MatrixSetting::MATRIX_SETTING_ID_COUNT:
                break;
            }
            retVal[type] = value;
            
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
        root["modes"][0] = Json::Value();
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
    
    std::string getSettings(MatrixMode const * mode, unsigned int modeIndex)
    {
        // Root value for creating the JSON string
        Json::Value root;
        
        // Get the mode object
        root["mode"] = getModeJSON(mode, modeIndex);
        
        // Get the setting list from the mode
        std::vector<MatrixSetting *> const & settingList =
            mode->getSettings();
    
        // Get the setting object for each setting in the list
        root["settings"][0] = Json::Value();
        for (unsigned int i = 0; i < settingList.size(); i++)
        {
            root["settings"][i] = getSettingJSON(settingList.at(i), i);
        }
    
        // Write the JSON to a string
        Json::StyledWriter writer;
        return writer.write(root);
    }
}