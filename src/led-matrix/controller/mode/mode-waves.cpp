#include <led-matrix/controller/mode/mode-waves.hpp>

#include <cmath>
#include <stdexcept>

MatrixMode::MatrixModeID const MatrixModeWaves::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_WAVES;
long int const MatrixModeWaves::s_DELAY_MS = 10;
std::string const MatrixModeWaves::s_NAME_STR =
    "Waves mode";
std::string const MatrixModeWaves::s_DESCRIPTION_STR =
    "Display an interesting \"waves\" effect on the matrix";
    
MatrixModeWaves::MatrixModeWaves(MatrixDriver * driver)
    : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
    , m_timeStep(0)
{
}

void MatrixModeWaves::begin()
{
    MatrixMode::begin();
    
    // Reset the timestep so the animation will start over
    m_timeStep = 0;
}

void MatrixModeWaves::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // "Speed" factor determines the difference in frequency for adjacent
        // columns.  The speed factor is inverted - increasing the value
        // decreases the frequency difference.
        // This is currently a fixed value because the standard speed setting
        // which sets the update delay will work to change the speed.
        static unsigned int const speed = 300;

        // The whole display will be overwritten using the shifting plotLevel
        m_driver->clearAllPixels();
        
        for (float col = 0; col < m_driver->COLUMNS; col++)
        {
            // Get the current sin value for the column
            // Result range: [0, 1]
            float level = sin(m_timeStep * (col / speed + 0.08)) / 2.0 + 0.5;
            // Rescale it to 0-100 for plotLevel
            // Result range: [0, 100]
            level *= 100;
            // Shift the column in from the left - column indexes will be
            // correct when the shift is complete (col 0 will be the leftmost).
            plotLevel(level, MATRIX_MODE_PLOT_TYPE_SHIFTING, false);
        }
        
        // The display has been written - update the actual output
        m_driver->update();
        
        // Move to the next step in the animation
        m_timeStep++;
    }
}