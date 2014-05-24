#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#include "matrix-debug.h"
#include "matrix-driver.h"
#include "matrix-driver-console.h"
#include "matrix-controller.h"

#define PROGRAM_NAME ("led_matrix")

static void usage()
{
    fprintf(stderr, "Usage: %s [-e]\n"""
    "    -e  Enable matrix emulation mode.  In this mode, the LED matrix is emulated\n"""
    "        onscreen instead of driving the actual matrix.\n"""
    "\n",
    PROGRAM_NAME);
}

/* main
 * Performs command-line parsing and high-level state machine control.
 */
int main (int argc, char * argv[])
{
    // Default values for options which may be changed with arguments
    bool emulate = false;
    
    // Parse command-line arguments
    int c;
    while ((c = getopt(argc, argv, "e")) != -1)
    {
        switch (c)
        {
        case 'e':
            emulate = true;
            break;
        
        default:
            fprintf(stderr, "Unknown option -%c.\n", optopt);
            usage();
            exit(EXIT_FAILURE);
        }
    }
    
    DBG_INIT();
    DBG_PRINTF("Starting in debug mode.\n");
    
    // Create the appropriate driver type
    MatrixDriver * driver;
    if (emulate)
    {
        DBG_PRINTF("Starting in emulation mode.\n");
        driver = new MatrixDriverConsole();
    }
    else
    {
        DBG_PRINTF("Matrix control not implemented, use -e.\n");
        exit(EXIT_FAILURE);
    }
    DBG_PRINTF("Created driver.\n");
    
    // Create matrix controller
    MatrixController controller = MatrixController(driver);
    DBG_PRINTF("Created controller.\n");
    
    DBG_PRINTF("Testing pixels.\n");
    
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 500000000;
    
    // Turn on all pixels
    for (size_t x = 0; x < driver->COLUMNS; x++)
    {
        for (size_t y = 0; y < driver->ROWS; y++)
        {
            driver->setPixel(x, y);
        }
    }
    driver->update();
    sleep(1);
    
    // Write a checkerboard pattern then toggle all pixels a few times
    for (size_t y = 0; y < driver->ROWS; y++)
    {
        for (size_t x = y % 2; x < driver->COLUMNS; x += 2)
        {
            driver->clearPixel(x, y);
        }
    }
    driver->update();
    for (int i = 0; i < 3; i++)
    {
        nanosleep(&delay, NULL);
        for (size_t y = 0; y < driver->ROWS; y++)
        {
            for (size_t x = 0; x < driver->COLUMNS; x++)
            {
                driver->togglePixel(x, y);
            }
        }
        driver->update();
    }
    
    // Clear all pixels to end the test
    nanosleep(&delay, NULL);
    controller.enterIdle();
    controller.update();
    DBG_PRINTF("Done testing, exit in 1 second.\n");
    sleep(1);
    
    // Free allocated MatrixDriver
    delete driver;
    
    exit(EXIT_SUCCESS);
}