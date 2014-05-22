/* matrix-driver - defines an abstract class describing matrix drivers (i.e. SPI
 * based driver chip, onscreen emulator, etc.).
 */
#ifndef _MATRIX_DRIVER_H
#define _MATRIX_DRIVER_H

#include "matrix-debug.h"

class MatrixDriver
{
public:
    // Size of the matrix being driven
    static size_t const COLUMNS = 24;
    static size_t const ROWS = 16;
    
    // Create and initialize MatrixDriver
    MatrixDriver();
    
    // Get and set pixel states (true/set = on, false/clear = off)
    // Setters do not update the actual matrix!
    void setPixel(size_t col, size_t row);
    void clearPixel(size_t col, size_t row);
    bool getPixel(size_t col, size_t row) const;
    
    // Update the actual matrix state
    virtual void update() = 0;
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixDriver();

protected:
    // Flag to indicate if the state has changed since it was last displayed
    bool stateChanged;
    
    // Check if coordinates are within matrix bounds
    bool coordsValid(size_t col, size_t row) const;
    
private:
    // Matrix state
    bool matrix[COLUMNS][ROWS];
    // Update matrix state entry
    void assignPixel(size_t col, size_t row, bool value);
};

#endif