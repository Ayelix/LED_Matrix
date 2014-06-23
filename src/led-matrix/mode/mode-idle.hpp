/* matrix-mode - abstract class to represent modes of operation for the matrix.
 */
#ifndef _MATRIX_MODE_IDLE_H
#define _MATRIX_MODE_IDLE_H

#include <led-matrix/mode/mode.hpp>

class MatrixModeIdle : public MatrixMode
{
public:
    MatrixModeIdle(MatrixDriver * driver)
     : MatrixMode(MATRIX_MODE_ID_IDLE, s_delayMs, driver) {};

    void update();
    
private:
    // Idle mode update delay
    static long int s_delayMs;
};

#endif