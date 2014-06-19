/* matrix-webserver-launcher - Helper functions to launch the webserver.  These
 * functions take a long time to compile.  They are pulled out of the main file
 * so that they don't have to be recompiled whenever the main file changes.
 */

#include "../matrix-controller.h"

// Start the webserver listening on the given port
void launchWebserver(unsigned int port);

// Stop the webserver started with launchWebserver
void stopWebserver();

// Get a mode change from the webserver queue.  Returns the mode popped from
// to top of the queue or MATRIX_CONTROLLER_MODE_COUNT if queue is empty.
MatrixController::ControllerMode popWebserverModeChange();