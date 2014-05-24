#include "matrix-controller.h"

#include "matrix-debug.h"

MatrixController::MatrixController(MatrixDriver * const driver)
: driver(driver)
{
    // Begin in idle state
    enterIdle();
}

void MatrixController::update()
{
    switch (state)
    {
    case MATRIX_CONTROLLER_STATE_IDLE:
    {
        // Don't do anything in idle state.  All pixels should already be off so
        // just wait for a state change.
        break;
    }
    
    // Invalid state(s) begin hereS
    case MATRIX_CONTROLLER_STATE_COUNT:
    {
        // Log the error and reset to idle
        DBG_PRINTF("Invalid MatrixController state in update: %d.\n", state);
        enterIdle();
        break;
    }
    }
}

void MatrixController::enterIdle()
{
    // Turn off all pixels, update driver, and update the current state
    driver->clearAllPixels();
    driver->update();
    state = MATRIX_CONTROLLER_STATE_IDLE;
}