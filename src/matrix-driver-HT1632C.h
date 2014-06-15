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
    
    // Reverse endianness of data
    void reverseEndian(void * data, size_t size);
};

#endif