/* matrix-webserver - Webserver to control matrix.  Based on Pion library.
 */
#ifndef _MATRIX_WEBSERVER_H
#define _MATRIX_WEBSERVER_H

#include <pion/http/server.hpp>
#include <boost/lockfree/queue.hpp>

#include "../matrix-controller.h"

class MatrixWebserver : public pion::http::server
{
public:
    // Create a webserver which will listen on the given port when started.
    MatrixWebserver(unsigned int port = 0);
    
    // Queue which will hold mode change commands received by the webserver.
    boost::lockfree::queue<MatrixController::ControllerMode> modeChangeQueue;

private:
    static std::string const kHTMLStart;
    static std::string const kHTMLEnd;
    
    /* Handle requests for / */
    void rootHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
};

#endif