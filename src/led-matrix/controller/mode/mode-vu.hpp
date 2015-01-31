/* matrix-mode-vu - MatrixMode implementation for vu meter mode.
 */
#ifndef _MATRIX_MODE_VU_H
#define _MATRIX_MODE_VU_H

#include <led-matrix/controller/mode/mode.hpp>

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
    
    // TODO - replace with a new setting type
    PlotType m_plotType;
};

#endif