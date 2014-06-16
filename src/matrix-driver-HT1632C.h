/* matrix-driver-HT1632C - Implementation of MatrixDriver which uses an HT1632C
 * over SPI to drive the matrix.
 */
#ifndef _MATRIX_DRIVER_HT1632C_H
#define _MATRIX_DRIVER_HT1632C_H

#include <cstdint>
#include <cmath>
 
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
    
    // Number of bytes sent each time the HT1632C display RAM is updated.
    // Calculated by:
    // 1. Start with total number of pixels (total number of data bits)
    // 2. Add number of command bits (3) and address bits (7)
    // 4. Divide remaining data bits by 8 to get number of bytes, rounding up
    //    to account for partially-used bytes
    // First byte:      Second byte:
    // MSB - 1          MSB - A1(0)
    //   6 - 0            6 - A0(0)
    //   5 - 1            5 - D0 (COM0,ROW0)
    //   4 - A6(0)        4 - D1 (COM1,ROW0)
    //   3 - A5(0)        3 - D2 (COM2,ROW0)
    //   2 - A4(0)        2 - D3 (COM3,ROW0)
    //   1 - A3(0)        1 - D0 (COM4,ROW0)
    //   0 - A2(0)        0 - D1 (COM5,ROW0)
    static size_t const HT1632C_NUM_DATA_BYTES =
        ceil(((ROWS*COLUMNS) + 3 + 7) / 8.0);
    
    // Array to hold display data sent to the HT1632C
    uint8_t displayData[HT1632C_NUM_DATA_BYTES];
    
    // Commands which may be sent to the HT1632C.  The values are the command
    // codes corresponding to the commands.
    typedef enum
    {
        // Power down system (initial system state)
        HT1632C_CMD_SYS_DIS     = 0b000000000,
        // Power up system
        HT1632C_CMD_SYS_EN      = 0b000000010,
        // Turn off LEDs (disable LED duty cycle generator)
        HT1632C_CMD_LED_OFF     = 0b000000100,
        // Turn on LEDs
        HT1632C_CMD_LED_ON      = 0b000000110,
        // Disable LED blinking (default state)
        HT1632C_CMD_BLINK_OFF   = 0b000010000,
        // Enable LED blinking (0.25s on, 0.25s off)
        HT1632C_CMD_BLINK_ON    = 0b000010010,
        // Set COM outputs to N-MOS (direct drive) with 16 columns
        HT1632C_CMD_COM_NMOS16  = 0b001001000
    } HT1632C_CMD;
    
    // Send a command to the HT1632C
    void sendCommand(HT1632C_CMD cmd);
    
    // Reverse endianness of data (treated as a single variable of 'size' bytes)
    void reverseEndian(void * data, size_t size);
};

#endif