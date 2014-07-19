#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#include <string>
#include <sstream>
#include <stdexcept>

#include <led-matrix/debug/debug.hpp>
#include <led-matrix/driver/driver.hpp>
#include <led-matrix/driver/driver-console.hpp>
#include <led-matrix/driver/driver-HT1632C.hpp>
#include <led-matrix/controller/controller.hpp>
#include <led-matrix/font/font.hpp>
#include <led-matrix/webserver/webserver-launcher.hpp>
#include <led-matrix/controller/mode/mode.hpp>
#include <led-matrix/controller/mode/setting/setting.hpp>

unsigned int const DEFAULT_PORT = 8080;
std::string const DEFAULT_FILES_DIR = "files";

static void usage(char const * const program_name)
{
    fprintf(stderr, "usage: %s [-es][-p port_number]\n"""
    "    -e  Enable matrix emulation mode.  In this mode, the LED matrix is emulated\n"""
    "        onscreen instead of driving the actual matrix.\n"""
    "    -s  Skip startup tests.\n"""
    "    -p  Specify the port on which the server will receive incoming connections.\n"""
    "        Default port: %u.\n"""
    "    -f  Specify the root directory from which files should be served.\n"""
    "        Default directory: %s\n"""
    "\n",
    program_name, DEFAULT_PORT, DEFAULT_FILES_DIR.c_str());
}

/* main
 * Performs command-line parsing and high-level state machine control.
 */
int main (int argc, char * argv[])
{
    // Default values for options which may be changed with arguments
    bool emulate = false;
    bool runTests = true;
    unsigned int port = DEFAULT_PORT;
    std::string filesDir = DEFAULT_FILES_DIR;
    
    // Parse command-line arguments
    int c;
    while ((c = getopt(argc, argv, "esp:f:")) != -1)
    {
        switch (c)
        {
        case 'e':
        {
            emulate = true;
            break;
        }
        
        case 's':
        {
            runTests = false;
            break;
        }
        
        case 'p':
        {
            std::string const arg = optarg;
            std::stringstream optStream(arg);
            if ( !(optStream >> port) )
            {
                fprintf(stderr, "Unable to parse port number \"%s\".\n", optarg);
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        }
        
        case 'f':
        {
            filesDir = optarg;
            break;
        }
        
        default:
        {
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
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
        catch (std::runtime_error const & error)
        {
            DBG_PRINTF("Unable to create driver.\n");
            exit(EXIT_FAILURE);
        }
    }
    DBG_PRINTF("Created driver.\n");
    
    // Create matrix controller
    MatrixController controller(driver);
    DBG_PRINTF("Created controller.\n");
    
    // Start the webserver
    launchWebserver(port, filesDir, &controller);
    DBG_PRINTF("Started webserver on port %u serving files from %s.\n", port,
        filesDir.c_str());
    
    // Run startup tests if appropriate
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
        nanosleep(&delay, NULL);
        nanosleep(&delay, NULL);
        nanosleep(&delay, NULL);
        nanosleep(&delay, NULL);
        
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
    
        // Build a test string containing all characters in the font
        std::string testString = std::string();
        for (char c = font_min; c <= font_max; c++)
        {
            testString += c;
        }
        testString += ' ';
        
        // Print out information about all modes and their settings
        DBG_PRINTF("\n");
        std::vector<MatrixMode *> const & modeList = controller.getModes();
        std::vector<MatrixMode *>::const_iterator modeIter = modeList.begin();
        unsigned int modeIndex = 0;
        for ( ; modeIter < modeList.end(); modeIter++, modeIndex++)
        {
            DBG_PRINTF("Mode %2d: %s - %s.\n", modeIndex,
                (*modeIter)->getName().c_str(),
                (*modeIter)->getDescription().c_str());
            DBG_PRINTF("Settings:\n");
            
            std::vector<MatrixSetting *> const & settingsList =
                (*modeIter)->getSettings();
            std::vector<MatrixSetting *>::const_iterator settingIter =
                settingsList.begin();
            unsigned int settingIndex = 0;
            for ( ; settingIter < settingsList.end()
                ; settingIter++, settingIndex++)
            {
                DBG_PRINTF("         %2d: %s - %s.\n", settingIndex,
                    (*settingIter)->getName().c_str(),
                    (*settingIter)->getDescription().c_str());
            }
        }
        DBG_PRINTF("\n");
        
        DBG_PRINTF("Done testing, starting main loop in 1 second.\n");
        nanosleep(&delay, NULL);
        nanosleep(&delay, NULL);
    }
    
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
                    //controller.setText(text);
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
    
    // Stop the webserver
    stopWebserver();
    
    exit(EXIT_SUCCESS);
}