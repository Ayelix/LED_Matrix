/* setting-plot-type - MatrixSetting implementation for a MatrixMode::PlotType
   including the fill setting.
 */
#ifndef _MATRIX_SETTING_PLOT_TYPE_H
#define _MATRIX_SETTING_PLOT_TYPE_H

#include <led-matrix/controller/mode/setting/setting.hpp>
#include <led-matrix/controller/mode/mode.hpp> // for MatrixMode::PlotType

#include <vector>

// TODO cyclesetting - update base class
class MatrixSettingPlotType : public MatrixSetting
{
public:
    MatrixSettingPlotType(std::string const & name, 
        std::string const & description);
    
    // Struct to contain both the plot type and fill settings
    struct PlotTypeSettingValue
    {
        MatrixMode::PlotType plotType;
        bool fill;
    };
    
    // Get the current setting value
    PlotTypeSettingValue getValue() const;
    // Get the index of the current setting value
    size_t getValueIndex() const;
    // Set the current setting value by index in the list provided by
    // getValues().  If the provided index exceeds the maximum, it will be set
    // to the maximum.
    void setValue(size_t valueIndex);
    // Move to the next setting value.  Returns the index of the new value in
    // the list provided by getValues().
    size_t nextValue();
    
    // Get *a copy of* the list of values available for this setting
    std::vector<PlotTypeSettingValue> getValues() const;
    
private:
    static MatrixSetting::MatrixSettingID s_settingID;
    
    // List of available values
    std::vector<PlotTypeSettingValue> m_values;
    // Index of current value in m_values;
    size_t m_currentValueIndex;
};

#endif