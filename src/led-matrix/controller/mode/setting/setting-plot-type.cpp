#include <led-matrix/controller/mode/setting/setting-plot-type.hpp>

MatrixSetting::MatrixSettingID MatrixSettingPlotType::s_settingID
    = MATRIX_SETTING_ID_PLOT_TYPE;

MatrixSettingPlotType::MatrixSettingPlotType(std::string const & name, 
    std::string const & description)
    : MatrixSetting(s_settingID, name, description)
{}

MatrixSettingPlotType::PlotTypeSettingValue 
MatrixSettingPlotType::getValue() const
{
    return PlotTypeSettingValue();
}

size_t MatrixSettingPlotType::getValueIndex() const
{
    return 0;
}

void MatrixSettingPlotType::setValue(size_t valueIndex)
{
    
}

size_t MatrixSettingPlotType::nextValue()
{
    return 0;
}

std::vector<MatrixSettingPlotType::PlotTypeSettingValue> 
MatrixSettingPlotType::getValues() const
{
    return std::vector<MatrixSettingPlotType::PlotTypeSettingValue>();
}