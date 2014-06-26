#include <led-matrix/controller/mode/mode-idle.hpp>

MatrixMode::MatrixModeID MatrixModeIdle::s_modeID =
    MatrixMode::MATRIX_MODE_ID_IDLE;
// Idle mode update delay is long because there is nothing to update
long int MatrixModeIdle::s_delayMs = 500;
std::string const MatrixModeIdle::s_nameStr =
    "Idle mode";
std::string const MatrixModeIdle::s_descriptionStr =
    "All pixels are turned off.";

void MatrixModeIdle::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // Nothing to do
    }
}