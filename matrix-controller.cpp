#include "matrix-controller.h"

#include <cmath>

#include "matrix-debug.h"
#include "matrix-timer.h"

std::string const MatrixController::TEXT_DEFAULT = "Text mode! ";
long int const MatrixController::TEXT_SCROLL_DELAY_MS = 50;
long int const MatrixController::VU_UPDATE_DELAY_MS = 10;

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
    enterMode(static_cast<ControllerMode>(intMode));
}

void MatrixController::enterMode(ControllerMode mode)
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
    
    // Invalid mode(s) begin here
    case MATRIX_CONTROLLER_MODE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid mode in MatrixController::enterMode(%d).\n", mode);
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

void MatrixController::setText(std::string const & text)
{
    // Append a space at the end to add space when scrolling restarts
    scrollingText = text + ' ';
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
        
        static unsigned int level = 0;
        static PlotType type = MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL;
        
        level++;
        if (level > 100)
        {
            level = 0;
            int intType = static_cast<int>(type) + 1;
            if (intType >= MATRIX_CONTROLLER_PLOT_TYPE_COUNT)
            {
                intType = 0;
            }
            type = static_cast<PlotType>(intType);
        }
        
        plotLevel(level, type);
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

void MatrixController::plotLevel(unsigned int level, PlotType type)
{
    switch (type)
    {
    case MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL:
    {
        // Scale the level to get the number of rows to turn on
        level = round((double)(level * driver->ROWS) / 100.0);
        
        // Turn on the approriate rows
        driver->clearAllPixels();
        for (size_t row = (driver->ROWS - level); row < driver->ROWS; row++)
        {
            for (size_t col = 0; col < driver->COLUMNS; col++)
            {
                driver->setPixel(col, row);
            }
        }
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_HORIZONTAL:
    {
        // Scale the level to get the number of columns to turn on
        level = round((double)(level * driver->COLUMNS) / 100.0);
        
        // Turn on the approriate columns
        driver->clearAllPixels();
        for (size_t col = 0; col < level; col++)
        {
            for (size_t row = 0; row < driver->ROWS; row++)
            {
                driver->setPixel(col, row);
            }
        }
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_COUNT:
    {
        DBG_PRINTF("Invalid plot type in MatrixController::plotLevel(%d, %d).\n",
            level, type);
    }
    }
    
    driver->update();
}