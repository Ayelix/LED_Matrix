#include <led-matrix/webserver/webserver.hpp>

#include <vector>
#include <sstream>

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
    add_resource("/getModes",
        boost::bind(&MatrixWebserver::getModesHandler, this, _1, _2));
    add_resource("/setMode",
        boost::bind(&MatrixWebserver::setModeHandler, this, _1, _2));
    add_resource("/getSettings",
        boost::bind(&MatrixWebserver::getSettingsHandler, this, _1, _2));
    add_resource("/setSetting",
        boost::bind(&MatrixWebserver::setSettingHandler, this, _1, _2));
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
    
    writer->write("Nothing to do here yet.\n");
    
    r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
    r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

    writer->send();
}

void MatrixWebserver::getModesHandler(pion::http::request_ptr& httpRequest,
    pion::tcp::connection_ptr& tcpConn)
{
    pion::http::response_writer_ptr writer(
        pion::http::response_writer::create(
            tcpConn, 
            *httpRequest, 
            boost::bind(&pion::tcp::connection::finish, tcpConn)));
    pion::http::response& r = writer->get_response();
    
    // Write the getModes JSON response
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
            // Update the controller's mode
            m_controller->setMode(modeInt);
            
            // Write the getModes JSON response (setMode has the same response
            // as getModes)
            r.set_content_type("application/json");
            writer->write(JSONUtils::getModes(m_controller));
            
            r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
            r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
        }
    }

    writer->send();
}

void MatrixWebserver::getSettingsHandler(pion::http::request_ptr& httpRequest,
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
            // Get the mode for the given index
            MatrixMode * mode = m_controllerModes.at(modeInt);
            
            // Write the getSettings JSON response
            r.set_content_type("application/json");
            writer->write(JSONUtils::getSettings(mode, modeInt));
            
            r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
            r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
        }
    }

    writer->send();
}

void MatrixWebserver::setSettingHandler(pion::http::request_ptr& httpRequest,
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
 
    // Get 'mode' and 'setting' parameters
    pion::ihash_multimap& params = httpRequest->get_queries();
    pion::ihash_multimap::const_iterator modeIter = params.find("mode");
    pion::ihash_multimap::const_iterator settingIter = params.find("setting");
    
    if (modeIter != params.end())
    {
        // Parse the mode parameter
        std::string const modeStr = modeIter->second;
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
            // Get the mode for the given index and its setting list
            MatrixMode * mode = m_controllerModes.at(modeInt);
            std::vector<MatrixSetting *> const & settingList =
                mode->getSettings();
            
            // Parse the setting parameter
            std::string const settingStr = settingIter->second;
            unsigned int settingInt;
            std::istringstream iss(settingStr);
            iss >> settingInt;
            
            // If parsing failed or the setting index is invalid, return an
            // error
            if ( ((iss.rdstate() & std::istringstream::failbit) != 0) ||
                (settingInt >= settingList.size()))
            {
                writer->write("Invalid setting parameter: ");
                writer->write(settingStr);
            }
            else
            {
                // Get the setting for the given index
                //MatrixSetting * setting = settingList.at(settingInt);
                
                // Write the getSettings JSON response (setSetting has the same
                // response as getSettings).
                r.set_content_type("application/json");
                writer->write(JSONUtils::getSettings(mode, modeInt));
                
                r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
                r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);
            }
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