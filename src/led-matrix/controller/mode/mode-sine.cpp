#include <led-matrix/controller/mode/mode-sine.hpp>

#include <cmath>

MatrixMode::MatrixModeID const MatrixModeSine::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_SINE;
long int const MatrixModeSine::s_DELAY_MS = 25;
std::string const MatrixModeSine::s_NAME_STR =
    "Sine mode";
std::string const MatrixModeSine::s_DESCRIPTION_STR =
    "Plot a sine wave on the matrix.";

void MatrixModeSine::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        static unsigned int x = 0;
        
        // TODO: get period from settings
        x += 5;
        if (x > 360)
        {
            x = 0;
        }
        
        // Get the sine value in the range 0-100 rounded to an integer
        unsigned int const level = round( (sin(x*M_PI/180) + 1.0) * 50.0 );
        plotLevel(level, MATRIX_MODE_PLOT_TYPE_SHIFTING, false);
    }
}