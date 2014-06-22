/* matrix-driver-console - Implementation of MatrixDriver which displays the
 * matrix in an ncurses window.
 */
#ifndef _MATRIX_DRIVER_CONSOLE_H
#define _MATRIX_DRIVER_CONSOLE_H

#include <ncurses.h>
 
#include <led-matrix/driver/driver.hpp>

class MatrixDriverConsole : public MatrixDriver
{
public:
    // Create ncurses window and initialize MatrixDriverConsole
    MatrixDriverConsole();
    
    // Update the ncurses window to reflect the state set by setPixel()
    void update();

private:
    // Ncurses window handle
    WINDOW * matrixWindow;
};

#endif