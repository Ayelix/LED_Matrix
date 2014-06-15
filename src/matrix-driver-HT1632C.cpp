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
    spiFd = open("/dev/notadev", O_WRONLY);
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
    uint8_t mode = SPI_MODE_0;
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
    uint8_t bits;
    if (-1 == ioctl(spiFc, SPI_IOC_WR_BITS_PER_WORD, &bits)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to set SPI word size.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    // Verify SPI bits per word
    uint8_t bitsResult = 0;
    if (-1 == ioctl(spiFc, SPI_IOC_RD_BITS_PER_WORD, &bitsResult)
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
    unsigned int speed = 1000000; //1000000 = 1MHz
    if (-1 == ioctl(spiFc, SPI_IOC_WR_MAX_SPEED_HZ, &speed)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to set SPI clock speed.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    // Verify SPI clock speed
    unsigned int speedResult = 0;
    if (-1 == ioctl(spiFc, SPI_IOC_RD_MAX_SPEED_HZ, &speedResult)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): unable to get SPI clock speed.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    if (speed != speedResult)
    {
        DBG_PRINTF("MatrixDriverHT1632C::MatrixDriverHT1632C(): SPI clock speed incorrect.\n");
        throw std::system_error(EDOM, std::system_category());
    }
    
    //TODO: send setup commands to HT1632C
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