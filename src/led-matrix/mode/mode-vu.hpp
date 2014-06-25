/* matrix-mode-vu - MatrixMode implementation for vu meter mode.
 */
#ifndef _MATRIX_MODE_VU_H
#define _MATRIX_MODE_VU_H

#include <led-matrix/mode/mode.hpp>

class MatrixModeVu : public MatrixMode
{
public:
    MatrixModeVu(MatrixDriver * driver)
      : MatrixMode(s_modeID, s_nameStr, s_descriptionStr,
        s_delayMs, driver) {};

    void update();
    
private:
    static MatrixMode::MatrixModeID s_modeID;
    static long int s_delayMs;
    static std::string const s_nameStr;
    static std::string const s_descriptionStr;
    
    // Plot the given level (normalized to 0-100) on the matrix, using the given
    // plot type/style and optionally setting all pixels below the given level.
    typedef enum
    {
        // Bar-graph type plot going from bottom to top
        MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL,
        // Bar-graph type plot going from left to right
        MATRIX_CONTROLLER_PLOT_TYPE_HORIZONTAL,
        // Shift matrix left and plot the new level on the rightmost column
        MATRIX_CONTROLLER_PLOT_TYPE_SHIFTING,
        
        MATRIX_CONTROLLER_PLOT_TYPE_COUNT
    } PlotType;
    void plotLevel(unsigned int level,
        PlotType type = MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL,
        bool fill = false);
};

#endif