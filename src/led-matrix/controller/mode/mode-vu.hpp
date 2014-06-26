/* matrix-mode-vu - MatrixMode implementation for vu meter mode.
 */
#ifndef _MATRIX_MODE_VU_H
#define _MATRIX_MODE_VU_H

#include <led-matrix/controller/mode/mode.hpp>

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
};

#endif