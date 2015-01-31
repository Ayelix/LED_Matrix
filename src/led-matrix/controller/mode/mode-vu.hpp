/* matrix-mode-vu - MatrixMode implementation for vu meter mode.
 */
#ifndef _MATRIX_MODE_VU_H
#define _MATRIX_MODE_VU_H

#include <led-matrix/controller/mode/mode.hpp>
#include <led-matrix/controller/mode/setting/setting-plot-type.hpp>

class MatrixModeVu : public MatrixMode
{
public:
    MatrixModeVu(MatrixDriver * driver);

    void update();
    
private:
    static MatrixMode::MatrixModeID const s_MODE_ID;
    static long int const s_DELAY_MS;
    static std::string const s_NAME_STR;
    static std::string const s_DESCRIPTION_STR;
    
    // Setting to adjust the display mode for the VU meter
    MatrixSettingPlotType * m_plotTypeSetting;
};

#endif