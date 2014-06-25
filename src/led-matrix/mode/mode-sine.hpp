/* matrix-mode - MatrixMode implementation for plotting sine wave.
 */
#ifndef _MATRIX_MODE_SINE_H
#define _MATRIX_MODE_SINE_H

#include <led-matrix/mode/mode.hpp>

class MatrixModeSine : public MatrixMode
{
public:
    MatrixModeSine(MatrixDriver * driver)
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