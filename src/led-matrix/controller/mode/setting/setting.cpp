#include <led-matrix/controller/mode/setting/setting.hpp>

#include <stdexcept>
#include <sstream>

#include <led-matrix/debug/debug.hpp>

// Setting implementations
#include <led-matrix/controller/mode/setting/setting-string.hpp>
#include <led-matrix/controller/mode/setting/setting-ranged-double.hpp>

MatrixSetting::MatrixSetting(MatrixSettingID id, std::string const & name,
    std::string const & description)
 : m_nameStr(name), m_descriptionStr(description)
{
    checkID(id, "MatrixSetting::MatrixSetting()");
    m_id = id;
}

void MatrixSetting::destroySetting(MatrixSetting * setting)
{
    if (setting != NULL)
    {
        checkID(setting->getID(), "MatrixSetting::destroySetting()");
        
        // Cast to appropriate implementation and delete
        switch (setting->getID())
        {
            case MATRIX_SETTING_ID_STRING:
            {
                MatrixSettingString * const settingString =
                    (MatrixSettingString *)setting;
                delete settingString;
                return;
            }
            case MATRIX_SETTING_ID_RANGED_DOUBLE:
            {
                MatrixSettingRangedDouble * const settingRangedDouble =
                    (MatrixSettingRangedDouble *)setting;
                delete settingRangedDouble;
                return;
            }
            
            // No default case to preserve compiler warnings for unhandled enum
            // values
            case MATRIX_SETTING_ID_COUNT:
            break;
        }
    
        // If this point is reached, the setting is of an unknown type and should
        // have been caught by checkID.
        throw std::runtime_error("MatrixSetting::checkID() failure");
    }
    else
    {
        std::string const errorStr =
            "MatrixSetting::destroySetting(): setting must not be NULL";
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}

MatrixSetting * MatrixSetting::createSetting(MatrixSettingID id,
    std::string const & name, std::string const & description)
{
    checkID(id, "MatrixSetting::createSetting()");
    
    // Allocate appropriate implementation instance
    switch (id)
    {
        case MATRIX_SETTING_ID_STRING:
        {
            MatrixSettingString * const settingString =
                new MatrixSettingString(name, description);
            return settingString;
        }
        case MATRIX_SETTING_ID_RANGED_DOUBLE:
        {
            MatrixSettingRangedDouble * const settingRangedDouble =
                new MatrixSettingRangedDouble(name, description);
            return settingRangedDouble;
        }
        
        // No default case to preserve compiler warnings for unhandled enum
        // values
        case MATRIX_SETTING_ID_COUNT:
        break;
    }
    
    // If this point is reached, the setting is of an unknown type and should have
    // been caught by checkID.
    throw std::runtime_error("MatrixSetting::checkID() failure");
}

void MatrixSetting::checkID(MatrixSettingID id, std::string const & prefix)
{
    if (id >= MATRIX_SETTING_ID_COUNT)
    {
        std::ostringstream oss;
        oss << prefix << ": Invalid setting ID " << id;
        std::string const errorStr = oss.str();
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}
