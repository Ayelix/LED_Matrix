/* matrix-mode - MatrixMode implementation for idle mode (all pixels off).
 */
#ifndef _MATRIX_MODE_IDLE_H
#define _MATRIX_MODE_IDLE_H

#include <led-matrix/controller/mode/mode.hpp>

class MatrixModeIdle : public MatrixMode
{
public:
    MatrixModeIdle(MatrixDriver * driver)
      : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR,
        s_DELAY_MS, driver) {};

    void update();
    
private:
    static MatrixMode::MatrixModeID const s_MODE_ID;
    static long int const s_DELAY_MS;
    static std::string const s_NAME_STR;
    static std::string const s_DESCRIPTION_STR;
};

#endif