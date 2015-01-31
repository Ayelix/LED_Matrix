#include <led-matrix/controller/mode/mode-waves.hpp>

#include <cmath>
#include <stdexcept>

MatrixMode::MatrixModeID const MatrixModeWaves::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_WAVES;
long int const MatrixModeWaves::s_DELAY_MS = 25;
std::string const MatrixModeWaves::s_NAME_STR =
    "Waves mode";
std::string const MatrixModeWaves::s_DESCRIPTION_STR =
    "Display an interesting \"waves\" effect on the matrix";
    
MatrixModeWaves::MatrixModeWaves(MatrixDriver * driver)
    : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
{
}

void MatrixModeWaves::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // TODO - temporarily copying the basic sine wave animation
        
        static double x = 0;
        
        // Calculate the amount by which to increase x from the period setting
        x += 5 / (100 / 100);
        if (x > 360)
        {
            x = 0;
        }
        
        // Get the sine value in the range 0-100 rounded to an integer
        unsigned int const level = round( (sin(x*M_PI/180) + 1.0) * 50.0 );
        plotLevel(level, MATRIX_MODE_PLOT_TYPE_SHIFTING, false);
    }
}