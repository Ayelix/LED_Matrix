#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <system_error>

#include "matrix-debug.h"
#include "matrix-driver.h"
#include "matrix-driver-console.h"
#include "matrix-driver-HT1632C.h"
#include "matrix-controller.h"

#define PROGRAM_NAME ("led_matrix")

static void usage()
{
    fprintf(stderr, "Usage: %s [-es]\n"""
    "    -e  Enable matrix emulation mode.  In this mode, the LED matrix is emulated\n"""
    "        onscreen instead of driving the actual matrix.\n"""
    "    -s  Skip startup matrix test.\n"""
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
    bool runTests = true;
    
    // Parse command-line arguments
    int c;
    while ((c = getopt(argc, argv, "es")) != -1)
    {
        switch (c)
        {
        case 'e':
            emulate = true;
            break;
        
        case 's':
            runTests = false;
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
        DBG_PRINTF("Starting in emulation driver mode.\n");
        driver = new MatrixDriverConsole();
    }
    else
    {
        DBG_PRINTF("Starting in HT1632C driver mode.\n");
        try
        {
            driver = new MatrixDriverHT1632C();
        }
        catch (const std::system_error & error)
        {
            DBG_PRINTF("Error: %s:%d - %s\n",
                error.code().category().name(), error.code().value(),
                error.code().message().c_str());
            exit(EXIT_FAILURE);
        }
    }
    DBG_PRINTF("Created driver.\n");
    
    // Create matrix controller
    MatrixController controller = MatrixController(driver);
    DBG_PRINTF("Created controller.\n");
    
    if (runTests)
    {
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
        nanosleep(&delay, NULL);
        for (int i = 0; i < 3; i++)
        {
            for (size_t y = 0; y < driver->ROWS; y++)
            {
                for (size_t x = 0; x < driver->COLUMNS; x++)
                {
                    driver->togglePixel(x, y);
                }
            }
            driver->update();
            nanosleep(&delay, NULL);
        }
        
        // Shift all pixels out to the left
        delay.tv_nsec /= 10;
        for (size_t i = 0; i < driver->COLUMNS; i++)
        {
            driver->shiftLeftAllPixels();
            driver->update();
            nanosleep(&delay, NULL);
        }
        
        DBG_PRINTF("Done testing, starting main loop in 1 second.\n");
        sleep(1);
    
        // Build a test string containing all characters in the font
        std::string testString = std::string();
        for (char c = ' '; c <= '~'; c++)
        {
            testString += c;
        }
        testString += ' ';
        controller.setText(testString);
    }
    
    controller.enterIdleMode();
    DBG_PRINTF("\nThe following options are available:\n");
    DBG_PRINTF("  q: quit\n");
    DBG_PRINTF("  t: update text mode string\n");
    DBG_PRINTF("Press any other key to change mode.\n\n");
    
    // Main loop
    int typedCh = ERR;
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000;
    while (true)
    {
        // Run controller's update method to progress current animation
        controller.update();
        
        // Check for key press
        if (ERR != (typedCh = getch()))
        {
            if ('q' == typedCh)
            {
                DBG_PRINTF("q key pressed, exiting main loop.\n");
                break;
            }
            else if ('t' == typedCh)
            {
                DBG_PRINTF("Enter string to use in text mode:\n");
                echo();
                nodelay(stdscr, FALSE);
                curs_set(1);
                char text[256];
                if (ERR != getnstr(text, 256))
                {
                    controller.setText(text);
                }
                else
                {
                    DBG_PRINTF("\nError reading input string, try again.\n");
                }
                noecho();
                nodelay(stdscr, TRUE);
                curs_set(0);
            }
            else
            {
                controller.nextMode();
            }
        }
        
        // Slow down the loop a bit
        nanosleep(&delay, NULL);
    }
    
    // Free allocated MatrixDriver
    delete driver;
    
    exit(EXIT_SUCCESS);
}