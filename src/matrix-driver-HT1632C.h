/* matrix-driver-HT1632C - Implementation of MatrixDriver which uses an HT1632C
 * over SPI to drive the matrix.
 */
#ifndef _MATRIX_DRIVER_HT1632C_H
#define _MATRIX_DRIVER_HT1632C_H
 
#include "matrix-driver.h"

class MatrixDriverHT1632C : public MatrixDriver
{
public:
    // Initialize driver for communication with HT1632C
    MatrixDriverHT1632C();
    // Shut down communication with HT1632C
    ~MatrixDriverHT1632C();
    
    // Update the display RAM on the HT1632C
    void update();

private:
    // SPI device file descriptor
    int spiFd;
};

#endif