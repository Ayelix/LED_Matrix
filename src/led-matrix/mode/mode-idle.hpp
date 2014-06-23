/* matrix-mode - abstract class to represent modes of operation for the matrix.
 */
#ifndef _MATRIX_MODE_IDLE_H
#define _MATRIX_MODE_IDLE_H

#include <led-matrix/mode/mode.hpp>

#include <string>

class MatrixModeIdle : public MatrixMode
{
public:
    MatrixModeIdle(MatrixDriver * driver)
      : MatrixMode(MATRIX_MODE_ID_IDLE, s_nameStr, s_descriptionStr,
        s_delayMs, driver) {};

    void update();
    
private:
    static long int s_delayMs;
    static std::string const s_nameStr;
    static std::string const s_descriptionStr;
};

#endif