#include <led-matrix/controller/mode/mode-vu.hpp>

MatrixMode::MatrixModeID MatrixModeVu::s_modeID =
    MatrixMode::MATRIX_MODE_ID_VU;
long int MatrixModeVu::s_delayMs = 5;
std::string const MatrixModeVu::s_nameStr =
    "Vu mode";
std::string const MatrixModeVu::s_descriptionStr =
    "Use the matrix as a VU meter.";

void MatrixModeVu::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        static bool msgPrinted = false;
        if (!msgPrinted)
        {
            DBG_PRINTF("VU Mode not implemented.  Demo cycles through """
                "available VU level display types.\n");
                msgPrinted = true;
        }
        
        static unsigned int level = 0;
        static int incr = 1;
        static bool fill = false;
        static PlotType type = MATRIX_MODE_PLOT_TYPE_VERTICAL;
        
        level += incr;
        if (level >= 100 || level <= 0)
        {
            incr = -incr;
            if (incr > 0)
            {
                int intType = static_cast<int>(type) + 1;
                if (intType >= MATRIX_MODE_PLOT_TYPE_COUNT)
                {
                    fill = !fill;
                    intType = 0;
                }
                type = static_cast<PlotType>(intType);
            }
        }
        
        plotLevel(level, type, fill);
    }
}