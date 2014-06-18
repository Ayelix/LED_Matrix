#include "matrix-webserver.h"

#include <pion/http/response_writer.hpp>

std::string const MatrixWebserver::kHTMLStart("<html><body>\n");
std::string const MatrixWebserver::kHTMLEnd("</body></html>\n");
 
MatrixWebserver::MatrixWebserver(unsigned int port)
    : pion::http::server(port)
{
    // Add a resource.
    add_resource("/",
        boost::bind(&MatrixWebserver::requestHandler, this, _1, _2));
}

void MatrixWebserver::requestHandler(pion::http::request_ptr& httpRequest,
    pion::tcp::connection_ptr& tcpConn)
{
    pion::http::response_writer_ptr writer(
        pion::http::response_writer::create(
            tcpConn, 
            *httpRequest, 
            boost::bind(&pion::tcp::connection::finish, tcpConn)));
    pion::http::response& r = writer->get_response();
 
    pion::ihash_multimap& params = httpRequest->get_queries();
 
    writer->write(kHTMLStart);

    pion::ihash_multimap::const_iterator paramIter = params.find("id");
    if (paramIter != params.end())
    {
        r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
        r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

        std::string id = paramIter->second;
        writer->write("Request id : ");
        writer->write(id);
    }
    else
    {
        r.set_status_code(pion::http::types::RESPONSE_CODE_BAD_REQUEST);
        r.set_status_message(pion::http::types::RESPONSE_MESSAGE_BAD_REQUEST);
    }

    writer->write(kHTMLEnd);
    writer->send();
}