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
    // Create a webserver which will listen on the given port when started,
    // serve files from the given directory (do not include a trailing '/'),
    // and will use the given MatrixController.
    MatrixWebserver(unsigned int port, std::string const & filesDir,
        MatrixController * controller);

private:
    // Root directory for file-serving
    std::string const m_filesDir;
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
    void setSettingHandler(pion::http::request_ptr& _httpRequest, 
        pion::tcp::connection_ptr& _tcpConn);
        
    // Helper to build a link in HTML
    static std::string buildLink(std::string const & href,
        std::string const & text);
};

#endif