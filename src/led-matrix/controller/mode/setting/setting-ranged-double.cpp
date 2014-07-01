#include <led-matrix/controller/mode/setting/setting-ranged-double.hpp>

#include <led-matrix/debug/debug.hpp>

MatrixSetting::MatrixSettingID MatrixSettingRangedDouble::s_settingID =
    MatrixSetting::MATRIX_SETTING_ID_RANGED_DOUBLE;
    
void MatrixSettingRangedDouble::setMin(double newMin)
{
    if (newMin > getMax())
    {
        newMin = getMax();
    }
    m_min = newMin;
}
    
void MatrixSettingRangedDouble::setMax(double newMax)
{
    if (newMax < getMin())
    {
        newMax = getMin();
    }
    m_max = newMax;
}

void MatrixSettingRangedDouble::setValue(double newValue)
{
    if (newValue < getMin())
    {
        newValue = getMin();
    }
    else if (newValue > getMax())
    {
        newValue = getMax();
    }
    m_value = newValue;
}