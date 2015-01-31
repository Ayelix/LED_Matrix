#include <led-matrix/controller/mode/mode-vu.hpp>

MatrixMode::MatrixModeID const MatrixModeVu::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_VU;
long int const MatrixModeVu::s_DELAY_MS = 5;
std::string const MatrixModeVu::s_NAME_STR =
    "Vu mode";
std::string const MatrixModeVu::s_DESCRIPTION_STR =
    "Use the matrix as a VU meter";

MatrixModeVu::MatrixModeVu(MatrixDriver * driver)
    : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
    , m_plotType(MATRIX_MODE_PLOT_TYPE_VERTICAL)
{
    // TODO - set up plot type setting
}

void MatrixModeVu::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // TODO - get the level from some measurement interface
        static unsigned int level = 0;
        level = (level >= 100) ? (0) : (level + 1);
        
        // TODO - get plot type from setting instead of m_plotType
        plotLevel(level, m_plotType, false);
        m_driver->update();
    }
}