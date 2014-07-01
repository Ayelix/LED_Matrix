/* setting.hpp - abstract class to represent various settings which may be
 * adjusted to control the behavior of matrix modes.
 */
#ifndef _MATRIX_SETTING_H
#define _MATRIX_SETTING_H

#include <string>

class MatrixSetting
{
public:
    // All available setting types
    typedef enum
    {
        // A setting for a string which may be modified
        MATRIX_SETTING_ID_STRING, //MatrixSettingString
        // A setting for a double which may take a value within a specific range
        MATRIX_SETTING_ID_RANGED_DOUBLE, //MatrixSettingRangedDouble
        
        // Add new settings above so the count remains correct
        MATRIX_SETTING_ID_COUNT //not a valid setting
    } MatrixSettingID;
    
    // Get the ID of a setting
    MatrixSettingID getID() const {return m_id;};
    
    // Get the name/description of a setting
    std::string getName() const {return m_nameStr;}
    std::string getDescription() const {return m_descriptionStr;}
    
    // Allocate memory for and create a new setting of the given type.
    // Returns NULL or throws std::runtime_error or std::invalid_argument on
    // failure.
    // The returned pointer should be freed with destroySetting().
    static MatrixSetting * createSetting(MatrixSettingID id,
        std::string const & name, std::string const & description);
    
    // Destroy the given setting, freeing memory allocated by createSetting().
    // Throws std::runtime_error or std::invalid_argument on failure.
    static void destroySetting(MatrixSetting * setting);
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixSetting() {};
    
protected:
    // Initialize a setting of the given type.
    // Constructor is protected because createSetting() should be used to create
    // the proper implementation for a setting.
    MatrixSetting(MatrixSettingID id, std::string const & name,
        std::string const & description);
    
private:
    // The ID of this setting
    MatrixSettingID m_id;
    // String name of this setting
    std::string const m_nameStr;
    // String description of this setting
    std::string const m_descriptionStr;
    
    // Check an ID and throw an invalid_argument exception if it's invalid.
    // Error string is of the format:
    // "<prefix>: Invalid setting ID <id>"
    static void checkID(MatrixSettingID m_id, std::string const & prefix);
};

#endif