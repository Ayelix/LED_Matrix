/* matrix-mode - MatrixMode implementation for idle mode (all pixels off).
 */
#ifndef _MATRIX_MODE_IDLE_H
#define _MATRIX_MODE_IDLE_H

#include <led-matrix/controller/mode/mode.hpp>

class MatrixModeIdle : public MatrixMode
{
public:
    MatrixModeIdle(MatrixDriver * driver)
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