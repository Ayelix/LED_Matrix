#include <led-matrix/controller/controller.hpp>

#include <cmath>

#include <led-matrix/debug/debug.hpp>
#include <led-matrix/timer/timer.hpp>

long int const MatrixController::ANIMATION_DELAY_MS = 25;
std::string const MatrixController::TEXT_DEFAULT = "Text mode! ";
long int const MatrixController::TEXT_SCROLL_DELAY_MS = 50;
long int const MatrixController::VU_UPDATE_DELAY_MS = 5;

MatrixController::MatrixController(MatrixDriver * const driver)
: driver(driver)
{
    // Begin in idle mode
    enterIdleMode();
    
    // Initialize text mode string
    setText(TEXT_DEFAULT);
}

/*******************************************************************************
 * Mode control methods
 ******************************************************************************/

void MatrixController::update()
{
    switch (mode)
    {
    case MATRIX_CONTROLLER_MODE_IDLE:
    {
        // Don't do anything in idle mode.  All pixels should already be off so
        // just wait for a mode change.
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_TEXT:
    {
        updateTextMode();
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_VU:
    {
        updateVUMode();
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_SINE:
    {
        updateSineMode();
        break;
    }
    
    // Invalid mode(s) begin here
    case MATRIX_CONTROLLER_MODE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid mode in MatrixController::update(): %d.\n", mode);
        enterIdleMode();
        break;
    }
    }
}

void MatrixController::nextMode()
{
    int intMode = static_cast<int>(mode) + 1;
    if (intMode >= MATRIX_CONTROLLER_MODE_COUNT)
    {
        intMode = 0;
    }
    setMode(static_cast<ControllerMode>(intMode));
}

void MatrixController::setMode(ControllerMode mode)
{
    switch(mode)
    {
    case MATRIX_CONTROLLER_MODE_IDLE:
    {
        enterIdleMode();
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_TEXT:
    {
        enterTextMode();
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_VU:
    {
        enterVUMode();
        break;
    }
    
    case MATRIX_CONTROLLER_MODE_SINE:
    {
        enterSineMode();
        break;
    }
    
    // Invalid mode(s) begin here
    case MATRIX_CONTROLLER_MODE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid mode in MatrixController::setMode(%d).\n", mode);
        enterIdleMode();
        break;
    }
    }
}
    
/*******************************************************************************
 * Idle mode methods
 ******************************************************************************/

void MatrixController::enterIdleMode()
{
    // Turn off all pixels, update driver, and update the current mode
    driver->clearAllPixels();
    driver->update();
    mode = MATRIX_CONTROLLER_MODE_IDLE;
    DBG_PRINTF("Entered mode: idle.\n");
}

/*******************************************************************************
 * Text mode methods
 ******************************************************************************/

void MatrixController::enterTextMode(std::string const & text)
{
    // Update the current text then call the no-parameter version
    setText(text);
    enterTextMode();
}

void MatrixController::enterTextMode()
{
    // Turn off all pixels and update driver
    driver->clearAllPixels();
    driver->update();
    // Reset the text position
    scrollingTextPosition = scrollingText.begin();
    currentCharPosition = 0;
    // Start the scroll timer
    MatrixTimer::startTimer(TEXT_SCROLL_DELAY_MS);
    // Update the mode
    mode = MATRIX_CONTROLLER_MODE_TEXT;
    DBG_PRINTF("Entered mode: text, current text \"%s\".\n",
        scrollingText.c_str());
}

void MatrixController::setText(std::string text)
{
    // Verify the input string
    std::string::iterator i = text.begin();
    while (i != text.end())
    {
        if ( (*i < font_min) || (*i > font_max))
        {
            DBG_PRINTF("MatrixController::setText(): removing illegal character %c from input string.\n",
                *i);
            i = text.erase(i);
        }
        else
        {
            i++;
        }
    }
    
    // Append a space at the end to add space when scrolling restarts
    scrollingText = text + ' ';
    
    DBG_PRINTF("Updated text mode string: \"%s\".\n",
        scrollingText.c_str());
    
    // If currently in text mode, reset the mode to present the new text
    if (MATRIX_CONTROLLER_MODE_TEXT == mode)
    {
        enterTextMode();
    }
}

void MatrixController::updateTextMode()
{
    if (MatrixTimer::checkTimer())
    {
        MatrixTimer::startTimer(TEXT_SCROLL_DELAY_MS);
        
        // Shift the matrix contents left to scroll the text
        driver->shiftLeftAllPixels();
        
        // Entire string has not been displayed
        if (scrollingTextPosition < scrollingText.end())
        {
            // Get the current character data
            FontChar const * const currentChar =
                getFontChar(*scrollingTextPosition);
            
            // Character is not fully displayed yet
            if (currentCharPosition < currentChar->width)
            {
                // Display the current column
                writeCharacterColumn(currentChar->data[currentCharPosition],
                    driver->COLUMNS - 1);
                // Move the current position to the next column
                currentCharPosition++;
            }
            // Character is fully displayed
            else
            {
                // Reset current character position for the next character
                currentCharPosition = 0;
                
                // Move to the next character
                scrollingTextPosition++;
                
                // Don't write anything to the rightmost column, let it be
                // a separator between characters.
            }
        }
        // Entire string has been displayed
        else
        {
            // Reset string position to the beginning
            scrollingTextPosition = scrollingText.begin();
                
            // Don't write anything to the rightmost column, let it be
            // a separator between the end and start of the string.
        }
        
        driver->update();
    }
}

/*******************************************************************************
 * VU meter mode methods
 ******************************************************************************/
void MatrixController::enterVUMode()
{
    // Turn off all pixels and update driver
    driver->clearAllPixels();
    driver->update();
    // Start the update timer
    MatrixTimer::startTimer(VU_UPDATE_DELAY_MS);
    // Update the mode
    mode = MATRIX_CONTROLLER_MODE_VU;
    DBG_PRINTF("Entered mode: vu.\n");
}

void MatrixController::updateVUMode()
{
    if (MatrixTimer::checkTimer())
    {
        MatrixTimer::startTimer(VU_UPDATE_DELAY_MS);
        
        static bool msgPrinted = false;
        if (!msgPrinted)
        {
            DBG_PRINTF("VU Mode not implemented.  Demo will cycle through """
                "available VU level display types.\n");
            msgPrinted = true;
        }
        
        static unsigned int level = 0;
        static int incr = 1;
        static bool fill = false;
        static PlotType type = MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL;
        
        level += incr;
        if (level >= 100 || level <= 0)
        {
            incr = -incr;
            if (incr > 0)
            {
                int intType = static_cast<int>(type) + 1;
                if (intType >= MATRIX_CONTROLLER_PLOT_TYPE_COUNT)
                {
                    fill = !fill;
                    intType = 0;
                }
                type = static_cast<PlotType>(intType);
            }
        }
        
        plotLevel(level, type, fill);
    }
}

/*******************************************************************************
 * Sine mode methods
 ******************************************************************************/
void MatrixController::enterSineMode()
{
    // Turn off all pixels and update driver
    driver->clearAllPixels();
    driver->update();
    // Start the update timer
    MatrixTimer::startTimer(ANIMATION_DELAY_MS);
    // Update the mode
    mode = MATRIX_CONTROLLER_MODE_SINE;
    DBG_PRINTF("Entered mode: sine.\n");
}

void MatrixController::updateSineMode()
{
    if (MatrixTimer::checkTimer())
    {
        MatrixTimer::startTimer(ANIMATION_DELAY_MS);
        
        static unsigned int x = 0;
        static bool fill = false;
        
        x += 5;
        if (x > 360)
        {
            fill = !fill;
            x = 0;
        }
        
        // Get the sine value in the range 0-100 rounded to an integer
        unsigned int const level = round( (sin(x*M_PI/180) + 1.0) * 50.0 );
        plotLevel(level, MATRIX_CONTROLLER_PLOT_TYPE_SHIFTING);
    }
}

/*******************************************************************************
 * Misc helper methods
 ******************************************************************************/
void MatrixController::writeCharacterColumn(uint16_t columnValue, size_t col)
{
    // Iterate through each pixel value.  Pixels are stored with the MSB as the
    // top pixel, but this loop goes through it from the LSB (bottom).
    for (int row = font_height - 1; row >= 0; row--)
    {
        // Write the current pixel value
        bool pixelVal = (columnValue & 0x01);
        driver->assignPixel(col, row, pixelVal);
        columnValue >>= 1;
    }
}

void MatrixController::plotLevel(unsigned int level, PlotType type, bool fill)
{
    // Coordinates for the rectangle which represents the plotted point.
    // Maximums are not inclusive, i.e. they must be 1 greater than the target.
    size_t col_min = 0, col_max = 0;
    size_t row_min = 0, row_max = 0;
    
    switch (type)
    {
    case MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL:
    {
        // Scale the level to get the row to turn on
        level = round((double)(level * (driver->ROWS - 2)) / 100.0);
        level = driver->ROWS - 2 - level;
        // Calculate the min/max values
        col_min = 0;
        col_max = driver->COLUMNS;
        row_min = level;
        row_max = (fill) ? (driver->ROWS) : (row_min + 2);
        // Clear all other pixels for this plot type
        driver->clearAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_HORIZONTAL:
    {
        // Scale the level to get the column to turn on
        level = round((double)(level * (driver->COLUMNS - 2)) / 100.0);
        //level = driver->COLUMNS - 2 - level;
        // Calculate the min/max values
        col_min = (fill) ? (0) : (level);
        col_max = level + 2;
        row_min = 0;
        row_max = driver->ROWS;
        // Clear all other pixels for this plot type
        driver->clearAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_SHIFTING:
    {
        // Scale the level to get the row to turn on
        level = round((double)(level * (driver->ROWS - 2)) / 100.0);
        level = driver->ROWS - 2 - level;
        // Calculate the min/max values
        col_min = driver->COLUMNS - 1;
        col_max = col_min + 1;
        row_min = level;
        row_max = (fill) ? (driver->ROWS) : (row_min + 2);
        // Shift all other pixels for this plot type
        driver->shiftLeftAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_COUNT:
    {
        DBG_PRINTF("Invalid plot type in MatrixController::plotLevel(%d, %d).\n",
            level, type);
    }
    }
    
    // Set the pixels according to the coordinates set above
    for (size_t row = row_min; row < row_max; row++)
    {
        for (size_t col = col_min; col < col_max; col++)
        {
            driver->setPixel(col, row);
        }
    }
    
    driver->update();
}