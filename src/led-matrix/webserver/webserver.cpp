#include <led-matrix/webserver/webserver.hpp>

#include <vector>
#include <sstream>

#include <pion/http/response_writer.hpp>
#include <led-matrix/webserver/json-utils.hpp>
 
MatrixWebserver::MatrixWebserver(unsigned int port,
    MatrixController * controller)
    : pion::http::server(port)
    , m_controller(controller)
    , m_controllerModes(m_controller->getModes())
{
    // Add resource handlers
    add_resource("/",
        boost::bind(&MatrixWebserver::rootHandler, this, _1, _2));
    add_resource("/setMode",
        boost::bind(&MatrixWebserver::setModeHandler, this, _1, _2));
    add_resource("/mode",
        boost::bind(&MatrixWebserver::modeHandler, this, _1, _2));
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
    
    // Return the getModes JSON
    r.set_content_type("application/json");
    writer->write(JSONUtils::getModes(m_controller));
    
    r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
    r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

    writer->send();
}

void MatrixWebserver::setModeHandler(pion::http::request_ptr& httpRequest,
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
        unsigned int modeInt;
        std::istringstream iss(modeStr);
        iss >> modeInt;
        
        // If parsing failed or the mode index is invalid, return an error
        if ( ((iss.rdstate() & std::istringstream::failbit) != 0) ||
            (modeInt >= m_controllerModes.size()))
        {
            writer->write("Invalid mode parameter: ");
            writer->write(modeStr);
        }
        else
        {   
            // Get the mode corresponding to the parameter
            MatrixMode const * const mode = m_controllerModes.at(modeInt);
            writer->write("Set mode to " + mode->getName() + ":\n");
            writer->write(mode->getDescription());
            
            // Update the controller's mode
            m_controller->setMode(modeInt);
            
            r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
            r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
        }
    }

    writer->send();
}

void MatrixWebserver::modeHandler(pion::http::request_ptr& httpRequest,
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
        unsigned int modeInt;
        std::istringstream iss(modeStr);
        iss >> modeInt;
        
        // If parsing failed or the mode index is invalid, return an error
        if ( ((iss.rdstate() & std::istringstream::failbit) != 0) ||
            (modeInt >= m_controllerModes.size()))
        {
            writer->write("Invalid mode parameter: ");
            writer->write(modeStr);
        }
        else
        {   
            // Get the mode corresponding to the parameter
            MatrixMode const * const mode = m_controllerModes.at(modeInt);
            
            // Update the controller's mode
            m_controller->setMode(modeInt);
            
            // Print out mode info
            writer->write(mode->getName() + " - " + mode->getDescription()
                + '\n');
            
            // Print out settings for the mode
            std::vector<MatrixSetting *> const & settingsList =
                mode->getSettings();
            std::vector<MatrixSetting *>::const_iterator settingIter =
                settingsList.begin();
            unsigned int settingIndex = 0;
            for ( ; settingIter < settingsList.end()
                ; settingIter++, settingIndex++)
            {
                writer->write((*settingIter)->getName() + " - " +
                    (*settingIter)->getDescription() + '\n');
            }
            
            r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
            r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
        }
    }

    writer->send();
}

std::string MatrixWebserver::buildLink(std::string const & href,
    std::string const & text)
{
    std::string const LINK_START = "<a href=\"";
    std::string const LINK_MID = "\">";
    std::string const LINK_END = "</a>";
    return LINK_START + href + LINK_MID + text + LINK_END;
}