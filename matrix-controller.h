/* matrix-controller - class to handle high-level control of the matrix, like
 * animations, timing, etc.
 */
#ifndef _MATRIX_CONTROLLER_H
#define _MATRIX_CONTROLLER_H

#include "matrix-driver.h"

#include <string>

class MatrixController
{
public:
    // Modes for the controller identify the current animation.
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_CONTROLLER_MODE_IDLE,
        // Scrolling text mode
        MATRIX_CONTROLLER_MODE_TEXT,
        
        // Add new modes above this line so the count stays correct.
        MATRIX_CONTROLLER_MODE_COUNT
    } ControllerMode;
    
    // Create a MatrixController which will use the given driver
    MatrixController(MatrixDriver * const driver);
    
    // Update current animation state if needed.  This method should be called
    // as frequently as possible (i.e. in an event loop) to keep the internal
    // timing as accurate as possible.
    void update();
    
    // Get the current mode
    ControllerMode getMode() {return mode;}
    // Move to the next mode
    void nextMode();
    
    /***************************************************************************
     * Idle mode methods
     **************************************************************************/
    // Turn off all pixels and sit idle
    void enterIdleMode();
    
    /***************************************************************************
     * Text mode methods
     **************************************************************************/
    // Turn off all pixels and enter text mode with or without specifying new
    // text to scroll.  If no text is specified, the last text is used.
    void enterTextMode(const std::string & text);
    void enterTextMode();
    // Update scrolling text.  If the current mode is not text mode, this will
    // set the text to be used next time text mode is entered.
    void setText(const std::string & text);
    
private:
    // MatrixDriver to be used by this controller
    MatrixDriver * const driver;
    
    // Current state
    ControllerMode mode;
    
    // Update current state.  Calls the appropriate enter*Mode() method.
    void enterMode(ControllerMode mode);
    
    /***************************************************************************
     * Text mode data members
     **************************************************************************/
    static const std::string TEXT_DEFAULT;
    std::string scrollingText;
};

#endif