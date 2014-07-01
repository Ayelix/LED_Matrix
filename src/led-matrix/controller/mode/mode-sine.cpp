#include <led-matrix/controller/mode/mode-sine.hpp>

#include <cmath>
#include <stdexcept>

MatrixMode::MatrixModeID const MatrixModeSine::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_SINE;
long int const MatrixModeSine::s_DELAY_MS = 25;
std::string const MatrixModeSine::s_NAME_STR =
    "Sine mode";
std::string const MatrixModeSine::s_DESCRIPTION_STR =
    "Plot a sine wave on the matrix";
    
MatrixModeSine::MatrixModeSine(MatrixDriver * driver)
    : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
{
    // Add the setting for sine wave period
    m_periodSetting = (MatrixSettingRangedDouble *) MatrixSetting::createSetting(
        MatrixSetting::MATRIX_SETTING_ID_RANGED_DOUBLE, "Period",
        "Period of the sine wave as a percentage of the default period, """
        "provided in the range 50-500%");
    if (NULL == m_periodSetting)
    {
        throw std::runtime_error(
            "MatrixModeSine::MatrixModeSine(): Unable to create period setting");
    }
    m_periodSetting->setMin(50);
    m_periodSetting->setMax(500);
    m_periodSetting->setValue(100);
    m_settings.push_back(m_periodSetting);
}

void MatrixModeSine::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        static double x = 0;
        
        // Calculate the amount by which to increase x from the period setting
        x += 5 / (m_periodSetting->getValue() / 100);
        if (x > 360)
        {
            x = 0;
        }
        
        // Get the sine value in the range 0-100 rounded to an integer
        unsigned int const level = round( (sin(x*M_PI/180) + 1.0) * 50.0 );
        plotLevel(level, MATRIX_MODE_PLOT_TYPE_SHIFTING, false);
    }
}