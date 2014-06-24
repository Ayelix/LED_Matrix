#include <led-matrix/webserver/webserver.hpp>

#include <pion/http/response_writer.hpp>
 
MatrixWebserver::MatrixWebserver(unsigned int port)
    : pion::http::server(port)
{
    // Add resource handlers
    add_resource("/",
        boost::bind(&MatrixWebserver::rootHandler, this, _1, _2));
}

void MatrixWebserver::rootHandler(pion::http::request_ptr& httpRequest,
    pion::tcp::connection_ptr& tcpConn)
{
    pion::http::response_writer_ptr writer(
        pion::http::response_writer::create(
            tcpConn, 
            *httpRequest, 
            boost::bind(&pion::tcp::connection::finish, tcpConn)));
    pion::http::response& r = writer->get_response();
    
    // Assume failure until the request is successfully handled
    r.set_status_code(pion::http::types::RESPONSE_CODE_BAD_REQUEST);
    r.set_status_message(pion::http::types::RESPONSE_MESSAGE_BAD_REQUEST);
 
    // Get 'mode' parameter
    pion::ihash_multimap& params = httpRequest->get_queries();
    pion::ihash_multimap::const_iterator paramIter = params.find("mode");
    
    if (paramIter != params.end())
    {
        // Parse the mode parameter
        std::string const modeStr = paramIter->second;
        writer->write("Not implemented.");
    }

    writer->send();
}