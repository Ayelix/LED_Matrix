#include <led-matrix/controller/mode/mode-sine.hpp>

#include <cmath>

MatrixMode::MatrixModeID MatrixModeSine::s_modeID =
    MatrixMode::MATRIX_MODE_ID_SINE;
long int MatrixModeSine::s_delayMs = 25;
std::string const MatrixModeSine::s_nameStr =
    "Sine mode";
std::string const MatrixModeSine::s_descriptionStr =
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