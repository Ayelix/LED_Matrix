#include <led-matrix/controller/mode/mode-idle.hpp>

MatrixMode::MatrixModeID const MatrixModeIdle::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_IDLE;
// Idle mode update delay is long because there is nothing to update
long int const MatrixModeIdle::s_DELAY_MS = 500;
std::string const MatrixModeIdle::s_NAME_STR =
    "Idle mode";
std::string const MatrixModeIdle::s_DESCRIPTION_STR =
    "All pixels are turned off";

void MatrixModeIdle::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // Nothing to do
    }
}