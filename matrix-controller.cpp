#include "matrix-controller.h"

#include <time.h>

#include "matrix-debug.h"
#include "matrix-timer.h"

const std::string MatrixController::TEXT_DEFAULT = "Text Mode";
const long int MatrixController::TEXT_SCROLL_DELAY_MS = 100;

MatrixController::MatrixController(MatrixDriver * const driver)
: driver(driver),
  // Text mode data members
  scrollingText(TEXT_DEFAULT)
{
    // Begin in idle mode
    enterIdleMode();
}

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
        static size_t textCol = 0;
        
        if (MatrixTimer::checkTimer())
        {
            MatrixTimer::startTimer(TEXT_SCROLL_DELAY_MS);
            
            driver->clearAllPixels();
            FontChar const * const fontChar = getFontChar('A');
            for (size_t col = 0; col < fontChar->width; col++)
            {
                const size_t curCol = (col + textCol) % driver->COLUMNS;
                writeCharacterColumn(fontChar->data[col], curCol);
            }
            driver->update();
        
            textCol++;
            if (textCol >= driver->COLUMNS) textCol = 0;
        }
        
        break;
    }
    
    // Invalid mode(s) begin here
    case MATRIX_CONTROLLER_MODE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid MatrixController mode in update: %d.\n", mode);
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

void MatrixController::enterTextMode(const std::string & text)
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
    // Start the scroll timer
    MatrixTimer::startTimer(TEXT_SCROLL_DELAY_MS);
    // Update the mode
    mode = MATRIX_CONTROLLER_MODE_TEXT;
    DBG_PRINTF("Entered mode: text, current text \"%s\".\n",
        scrollingText.c_str());
}

void MatrixController::setText(const std::string & text)
{
    scrollingText = text;
    // If currently in text mode, reset the mode to present the new text
    if (MATRIX_CONTROLLER_MODE_TEXT == mode)
    {
        enterTextMode();
    }
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
    
    // Invalid mode(s) begin here
    case MATRIX_CONTROLLER_MODE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid MatrixController mode in enterMode: %d.\n", mode);
        enterIdleMode();
        break;
    }
    }
}

void MatrixController::writeCharacterColumn(uint16_t columnValue, size_t col)
{
    // Iterate through each pixel value.  Pixels are stored with the MSB as the
    // top pixel, but this loop goes through it from the LSB (bottom).
    for (int row = 15; row >= 0; row--)
    {
        // Write the current pixel value
        bool pixelVal = (columnValue & 0x01);
        driver->assignPixel(col, row, pixelVal);
        columnValue >>= 1;
    }
}