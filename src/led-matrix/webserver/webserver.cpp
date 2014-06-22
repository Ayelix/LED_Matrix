#include <led-matrix/webserver/webserver.hpp>

#include <pion/http/response_writer.hpp>
 
MatrixWebserver::MatrixWebserver(unsigned int port)
    : pion::http::server(port)
    , modeChangeQueue(100)
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
        // This block uses the MatrixController::ControllerMode type and values
        // often.
        //using namespace MatrixController;
        
        // Parse the mode parameter
        std::string const modeStr = paramIter->second;
         MatrixController::ControllerMode mode =
             MatrixController::MATRIX_CONTROLLER_MODE_COUNT;
        if ("idle" == modeStr)
        {
            mode = MatrixController::MATRIX_CONTROLLER_MODE_IDLE;
        }
        else if ("text" == modeStr)
        {
            mode = MatrixController::MATRIX_CONTROLLER_MODE_TEXT;
        }
        else if ("vu" == modeStr)
        {
            mode = MatrixController::MATRIX_CONTROLLER_MODE_VU;
        }
        else if ("sine" == modeStr)
        {
            mode = MatrixController::MATRIX_CONTROLLER_MODE_SINE;
        }
        
        // If a mode was parsed, queue the mode change and respond OK
        if(MatrixController::MATRIX_CONTROLLER_MODE_COUNT != mode)
        {
            DBG_PRINTF("MatrixWebserver::rootHandler(): Queueing mode change \"%s\".\n",
                modeStr.c_str());
            writer->write("Got mode change ");
            writer->write(modeStr);
            
            while (!modeChangeQueue.push(mode));
            
            r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
            r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
        }
        else
        {
            writer->write("Unknown mode \"");
            writer->write(modeStr);
            writer->write("\"");
        }
    }

    writer->send();
}