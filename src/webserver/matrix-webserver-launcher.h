/* matrix-webserver-launcher - Helper functions to launch the webserver.  These
 * functions take a long time to compile.  They are pulled out of the main file
 * so that they don't have to be recompiled whenever the main file changes.
 */

// Start the webserver listening on the given port
void launchWebserver(unsigned int port);

// Stop the webserver started with launchWebserver
void stopWebserver();