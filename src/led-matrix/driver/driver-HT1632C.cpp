#include <led-matrix/driver/driver-HT1632C.hpp>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstdio>
#include <stdexcept>
#include <cerrno>
#include <cstring>

MatrixDriverHT1632C::MatrixDriverHT1632C()
{
    std::string const errorPrefix = "MatrixDriverHT1632C::MatrixDriverHT1632C(): ";
    std::string errorStr;
    
    // This driver (sloppily) depends on the values of ROWS and COLUMNS
    if ( (ROWS != 16) || (COLUMNS != 24) )
    {
        errorStr = errorPrefix + "WARNING - incorrect row/column size.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    
    // Initialize displayData by writing zeros then setting mode bits
    memset(&displayData, 0, HT1632C_NUM_DATA_BYTES);
    displayData[0] = 0b10100000;
    
    // Open the SPI device for writing
    spiFd = open("/dev/spidev0.0", O_WRONLY);
    if (spiFd < 0)
    {
        errorStr = errorPrefix + "unable to open SPI device.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);    
    }
    
    // Configure SPI clock mode
    // SPI modes copied from http://www.raspberry-projects.com/pi/programming-in-c/spi/using-the-spi-interface
    //SPI_MODE_0 (0,0) 	CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_1 (0,1) 	CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
    //SPI_MODE_2 (1,0) 	CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_3 (1,1) 	CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
    uint8_t mode = SPI_MODE_3;
    if (-1 == ioctl(spiFd, SPI_IOC_WR_MODE, &mode))
    {
        errorStr = errorPrefix + "unable to set SPI mode.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr); 
    }
    // Verify SPI clock mode
    uint8_t modeResult;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_MODE, &modeResult))
    {
        errorStr = errorPrefix + "unable to get SPI mode.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr); 
    }
    if (mode != modeResult)
    {
        errorStr = errorPrefix + "SPI mode incorrect.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr); 
    }
    
    // Configure SPI bits per word
    uint8_t bits = 8;
    if (-1 == ioctl(spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits))
    {
        errorStr = errorPrefix + "unable to set SPI word size.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    // Verify SPI bits per word
    uint8_t bitsResult = 0;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_BITS_PER_WORD, &bitsResult))
    {
        errorStr = errorPrefix + "unable to get SPI word size.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    if (bits != bitsResult)
    {
        errorStr = errorPrefix + "SPI word size incorrect.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    
    // Configure SPI clock speed
    unsigned int speed = 488000; //1000000 = 1MHz (HT1632C max)
    if (-1 == ioctl(spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    {
        errorStr = errorPrefix + "unable to set SPI clock speed.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    // Verify SPI clock speed
    unsigned int speedResult = 0;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_MAX_SPEED_HZ, &speedResult))
    {
        errorStr = errorPrefix + "unable to get SPI clock speed.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    if (speed != speedResult)
    {
        errorStr = errorPrefix + "SPI clock speed incorrect.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
    
    // Initialize the HT1632C
    sendCommand(HT1632C_CMD_SYS_DIS);
    sendCommand(HT1632C_CMD_COM_NMOS16);
    sendCommand(HT1632C_CMD_SYS_EN);
    //TODO: may need to clear all pixels and update before turning LEDs on
    sendCommand(HT1632C_CMD_LED_ON);
}

MatrixDriverHT1632C::~MatrixDriverHT1632C()
{
    // Close the SPI device file descriptor
    if (0 > close(spiFd))
    {
        DBG_PRINTF("MatrixDriverHT1632C::~MatrixDriverHT1632C(): unable to close SPI file descriptor:\n");
    }
}

void MatrixDriverHT1632C::update()
{
    // Do nothing if the state hasn't changed
    if (stateChanged)
    {
        // TODO: if this state-copying is too slow, consider overriding the 
        // state-related methods/members of MatrixDriver.
        
        // Update the displayData array to match the new state
        size_t bitOffset = 2; // Start 2 bits from MSB
        size_t arrayIndex = 1; // At index 1 of the array
        for (size_t row = 0; row < ROWS; row++)
        {
            for (size_t col = 0; col < COLUMNS; col++)
            {
                // Set or clear the bit corresponding to the pixel
                uint8_t const bitMask = ((uint8_t) 1) << (8 - bitOffset - 1);
                if (getPixel(col, row))
                {
                    displayData[arrayIndex] |= bitMask;
                }
                else
                {
                    displayData[arrayIndex] &= ~bitMask;
                }
                
                // Move to the next bit
                bitOffset = (bitOffset + 1) % 8;
                // Increment the array index if the byte is complete
                if (0 == bitOffset)
                {
                    arrayIndex++;
                }
            }
        }
        
        // Write the updated display data to the HT1632C
        if ((ssize_t)HT1632C_NUM_DATA_BYTES !=
            write(spiFd, &displayData, HT1632C_NUM_DATA_BYTES))
        {
            std::string const errorStr =
                "MatrixDriverHT1632C::update(): unable to write to SPI device.\n";
            DBG_PRINTF(errorStr.c_str());
            throw std::runtime_error(errorStr);
        }
        
        // Reset stateChanged flag now that the matrix has been updated
        stateChanged = false;
    }
    else
    {
        DBG_PRINTF("MatrixDriverHT1632C::update(): stateChanged is false, do nothing.\n");
    }
}

void MatrixDriverHT1632C::sendCommand(HT1632C_CMD cmd)
{
    // Build the command to send, starting with '100' for the command-mode bits
    uint16_t completeCommand = 0b1000000000000000;
    // Add in the command code bits, shifted up to leave the last 4 bits 0
    completeCommand |= static_cast<uint16_t>(cmd) << 4;
    // Reverse the endianness before sending
    reverseEndian(&completeCommand, sizeof(completeCommand));
    
    // Send the command
    if (sizeof(completeCommand) !=
        write(spiFd, &completeCommand, sizeof(completeCommand)))
    {
        std::string const errorStr =
            "MatrixDriverHT1632C::sendCommand(): unable to write to SPI device.\n";
        DBG_PRINTF(errorStr.c_str());
        throw std::runtime_error(errorStr);
    }
}

void MatrixDriverHT1632C::reverseEndian(void * const data, size_t const size)
{
    if (size % 2 != 0)
    {
        DBG_PRINTF("MatrixDriverHT1632C::reverseEndian called with odd size %zu.\n",
            size);
    }
    
    // Swap each byte from the ends into the middle
    char * first = (char *) data;
    char * last = ((char *) data) + size - 1;
    while (last > first)
    {
        // Swap the bytes
        char swap = *first;
        *first = *last;
        *last = swap;
        // Move pointers towards the middle of the data
        first++;
        last--;
    }
}