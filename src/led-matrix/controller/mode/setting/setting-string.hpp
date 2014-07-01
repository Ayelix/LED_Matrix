/* setting-string - MatrixSetting implementation for adjustable string
 */
#ifndef _MATRIX_SETTING_STRING_H
#define _MATRIX_SETTING_STRING_H

#include <led-matrix/controller/mode/setting/setting.hpp>

class MatrixSettingString : public MatrixSetting
{
public:
    MatrixSettingString(std::string const & name, 
        std::string const & description,
        std::string const & initialString = std::string())
        : MatrixSetting(s_settingID, name, description)
        , m_value(initialString) {};
    
    // Get/set the string
    std::string getString() const {return m_value;}
    void setString(std::string const & newString) {m_value = newString;}
    
private:
    static MatrixSetting::MatrixSettingID s_settingID;
    
    // The string which is controlled by this setting
    std::string m_value;
};

#endif