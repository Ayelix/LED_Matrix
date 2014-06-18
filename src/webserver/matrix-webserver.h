/* matrix-webserver - Webserver to control matrix.  Based on Pion library.
 */
#ifndef _MATRIX_WEBSERVER_H
#define _MATRIX_WEBSERVER_H

#include <pion/http/server.hpp>

class MatrixWebserver : public pion::http::server
{
public:
    /* Create a webserver which will listen on the given port when started. */
    MatrixWebserver(unsigned int port = 0);

private:
    static std::string const kHTMLStart;
    static std::string const kHTMLEnd;
    
    /**
     * Handle http requests.
     * @param _httpRequest the request
     * @param _tcpConn the connection
     */
    void requestHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
};

#endif