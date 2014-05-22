#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#include "matrix-debug.h"
#include "matrix-driver.h"
#include "matrix-driver-console.h"

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
    sleep(1);
    
    DBG_PRINTF("Testing pixels.\n");
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 070000000;
    for (size_t x = 0; x < driver->COLUMNS; x++)
    {
        for (size_t y = 0; y < driver->ROWS; y++)
        {
            driver->setPixel(x, y);
            driver->update();
            nanosleep(&t, NULL);
        }
    }
    for (size_t y = 0; y < driver->ROWS; y++)
    {
        for (size_t x = 0; x < driver->COLUMNS; x++)
        {
            driver->clearPixel(x, y);
            driver->update();
            nanosleep(&t, NULL);
        }
    }
    
    // Free allocated MatrixDriver
    delete driver;
    
    exit(EXIT_SUCCESS);
}