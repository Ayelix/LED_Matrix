/* matrix-webserver - Webserver to control matrix.  Based on Pion library.
 */
#ifndef _MATRIX_WEBSERVER_H
#define _MATRIX_WEBSERVER_H

#include <pion/http/server.hpp>
#include <boost/lockfree/queue.hpp>
#include <string>

#include <pion/http/response_writer.hpp>

#include <led-matrix/controller/controller.hpp>

class MatrixWebserver : public pion::http::server
{
public:
    // Create a webserver which will listen on the given port when started and
    // will use the given MatrixController.
    MatrixWebserver(unsigned int port, MatrixController * controller);

private:
    // MatrixController used by the webserver
    MatrixController * const m_controller;
    // List of available modes from the controller
    std::vector<MatrixMode *> const & m_controllerModes;
    
    // Request handlers
    void rootHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
    void getModesHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
    void setModeHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
    void getSettingsHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
        
    // Helper to build a link in HTML
    static std::string buildLink(std::string const & href,
        std::string const & text);
};

#endif