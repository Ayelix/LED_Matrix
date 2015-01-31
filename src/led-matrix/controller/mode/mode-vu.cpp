#include <led-matrix/controller/mode/mode-vu.hpp>
#include <led-matrix/controller/mode/setting/setting.hpp>

#include <stdexcept>

MatrixMode::MatrixModeID const MatrixModeVu::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_VU;
long int const MatrixModeVu::s_DELAY_MS = 5;
std::string const MatrixModeVu::s_NAME_STR =
    "Vu mode";
std::string const MatrixModeVu::s_DESCRIPTION_STR =
    "Use the matrix as a VU meter";

MatrixModeVu::MatrixModeVu(MatrixDriver * driver)
    : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
{
    // Add the setting for the plot type
    m_plotTypeSetting = (MatrixSettingPlotType *) MatrixSetting::createSetting(
        MatrixSetting::MATRIX_SETTING_ID_PLOT_TYPE, "Display Type",
        "Display type for the VU meter");
    if (NULL == m_plotTypeSetting)
    {
        throw std::runtime_error(
            "MatrixModeVu::MatrixModeVu(): Unable to create plot type setting");
    }
    m_settings.push_back(m_plotTypeSetting);
}

void MatrixModeVu::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // TODO - get the level from some measurement interface
        static unsigned int level = 0;
        level = (level >= 100) ? (0) : (level + 1);
        
        // Plot the current level using the plot type from the setting
        MatrixSettingPlotType::PlotTypeSettingValue plotType =
            m_plotTypeSetting->getValue();
        plotLevel(level, plotType.plotType, plotType.fill);
        m_driver->update();
    }
}