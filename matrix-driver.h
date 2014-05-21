/* matrix-driver - defines an abstract class describing matrix drivers (i.e. SPI
 * based driver chip, onscreen emulator, etc.).
 */
#include "matrix-debug.h"

class MatrixDriver
{
public:
    // Size of the matrix being driven
    static unsigned int const ROWS = 16;
    static unsigned int const COLS = 24;
    
    // Create a driver for the given matrix size
    MatrixDriver();
    
    // Get and set pixel states (true = on, false = off)
    // Does not update the actual matrix!
    void setPixel(size_t row, size_t col);
    bool getPixel(size_t row, size_t col) const;
    
    // Update the actual matrix state
    virtual void update() = 0;
    
    // Virtual destructor as recommended by:
    // http://stackoverflow.com/a/318137
    virtual ~MatrixDriver();

private:
    // Matrix state
    bool matrix[ROWS][COLS];
    // Flag to indicate if the state has changed since it was last displayed
    bool stateChanged;
};