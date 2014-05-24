#include "matrix-controller.h"

#include "matrix-debug.h"

const std::string MatrixController::TEXT_DEFAULT = "Text Mode";

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
        static bool printed = false;
        if (!printed)
        {
            DBG_PRINTF("Text mode not implemented.\n");
            printed = true;
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
    // Turn off all pixels, update driver, and update the current mode so the
    // matrix is ready to begin scrolling text.
    driver->clearAllPixels();
    driver->update();
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