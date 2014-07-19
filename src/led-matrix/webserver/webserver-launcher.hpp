/* matrix-webserver-launcher - Helper functions to launch the webserver.  These
 * functions take a long time to compile.  They are pulled out of the main file
 * so that they don't have to be recompiled whenever the main file changes.
 */
 
#include <led-matrix/controller/controller.hpp>

// Start the webserver
void launchWebserver(unsigned int port, std::string const & filesDir,
    MatrixController * controller);

// Stop the webserver started with launchWebserver
void stopWebserver();
