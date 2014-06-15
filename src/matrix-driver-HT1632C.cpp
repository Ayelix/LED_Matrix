#include "matrix-driver-HT1632C.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstdint>
#include <linux/spi/spidev.h>
#include <cstdio>
#include <system_error>

MatrixDriverHT1632C::MatrixDriverHT1632C()
{
    // Open the SPI device for writing
    spiFd = open("/dev/spidev0.0", O_WRONLY);
    if (spiFd < 0)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to open SPI device.\n");
        throw std::system_error(EDOM, std::system_category());
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
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to set SPI mode.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    // Verify SPI clock mode
    uint8_t modeResult;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_MODE, &modeResult))
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to get SPI mode.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    if (mode != modeResult)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): SPI mode incorrect.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    
    // Configure SPI bits per word
    uint8_t bits = 8;
    if (-1 == ioctl(spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits))
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to set SPI word size.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    // Verify SPI bits per word
    uint8_t bitsResult = 0;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_BITS_PER_WORD, &bitsResult))
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to get SPI word size.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    if (bits != bitsResult)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): SPI word size incorrect.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    
    // Configure SPI clock speed
    // TODO: increase clock speed (it's slowed down for the slow oscilloscope)
    unsigned int speed = 10000; //1000000 = 1MHz (HT1632C max)
    if (-1 == ioctl(spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to set SPI clock speed.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    // Verify SPI clock speed
    unsigned int speedResult = 0;
    if (-1 == ioctl(spiFd, SPI_IOC_RD_MAX_SPEED_HZ, &speedResult))
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to get SPI clock speed.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    if (speed != speedResult)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): SPI clock speed incorrect.\n");
        throw std::system_error(EDOM, std::system_category());
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
        //TODO: write to HT1632C display RAM
        DBG_PRINTF("MatrixDriverHT1632C::update(): not implemented.\n");
        /*std::string line;
        line += "----------------------------------------------------\n";
        for (size_t row = 0; row < ROWS; row++)
        {
            line += "--";
            for (size_t col = 0; col < COLUMNS; col++)
            {
                if (getPixel(col, row))
                {
                    line += "XX";
                }
                else
                {
                    line += "  ";
                }
            }
            line += "--\n";
        }
        line += "----------------------------------------------------\n";
        DBG_PRINTF(line.c_str());*/
        
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
        DBG_PRINTF("MatrixDriverHT1632C::sendCommand(): unable to write to SPI device.\n");
        throw std::system_error(EDOM, std::system_category());
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