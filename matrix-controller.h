/* matrix-controller - class to handle high-level control of the matrix, like
 * animations, timing, etc.
 */
#ifndef _MATRIX_CONTROLLER_H
#define _MATRIX_CONTROLLER_H

#include "matrix-driver.h"

class MatrixController
{
public:
    // Create a MatrixController which will use the given driver
    MatrixController(MatrixDriver * const driver);
    
    // Update current animation state if needed.  This method should be called
    // as frequently as possible (i.e. in an event loop) to keep the internal
    // timing as accurate as possible.
    void update();
    
    /***************************************************************************
     * State change methods
     **************************************************************************/
    void enterIdle();
    
private:
    // MatrixDriver to be used by this controller
    MatrixDriver * const driver;

    // States for the controller identify the current animation.
    typedef enum
    {
        // Idle/off state - all pixels off
        MATRIX_CONTROLLER_STATE_IDLE,
        
        // Add new states above this line so the count stays correct.
        MATRIX_CONTROLLER_STATE_COUNT
    } ControllerState;
    
    // Current state
    ControllerState state;
};

#endif