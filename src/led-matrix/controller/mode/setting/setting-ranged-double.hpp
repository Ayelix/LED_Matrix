/* setting-ranged-double - MatrixSetting implementation for a double within a
 * specific range
 */
#ifndef _MATRIX_SETTING_RANGED_DOUBLE_H
#define _MATRIX_SETTING_RANGED_DOUBLE_H

#include <led-matrix/controller/mode/setting/setting.hpp>

class MatrixSettingRangedDouble : public MatrixSetting
{
public:
    MatrixSettingRangedDouble(std::string const & name, 
        std::string const & description,
        double minimum = 0, double maximum = 100)
        : MatrixSetting(s_settingID, name, description)
        , m_min(minimum), m_max(maximum)
        , m_value(minimum) {};
    
    // Get the minimum and maximum allowed values (inclusive)
    double getMin() const {return m_min;}
    double getMax() const {return m_max;}
    // Set the minimum and maximum allowed values (inclusive).  If the provided
    // value is invalid (i.e. if newMin > getMax() or newMax < getMin()), the
    // value will be set equal to its opposite (i.e. newMin = getMax() or
    // newMax = getMin()).
    void setMin(double newMin);
    void setMax(double newMax);
    
    // Get the double value
    double getValue() const {return m_value;}
    // Set the double value.  The provided value is clipped to getMin() or
    // getMax() if it exceeds the allowed range.
    void setValue(double newValue);
    
private:
    static MatrixSetting::MatrixSettingID s_settingID;
    
    // Minimum and maximum allowed values (inclusive)
    double m_min, m_max;
    
    // The value which is controlled by this setting
    double m_value;
};

#endif