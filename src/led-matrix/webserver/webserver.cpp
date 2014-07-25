#include <led-matrix/webserver/webserver.hpp>

#include <vector>
#include <sstream>

#include <led-matrix/webserver/json-utils.hpp>
#include <led-matrix/webserver/file-serving.hpp>
#include <led-matrix/webserver/mime-types.hpp>

// Setting implementations
#include <led-matrix/controller/mode/setting/setting-string.hpp>
#include <led-matrix/controller/mode/setting/setting-ranged-double.hpp>
 
MatrixWebserver::MatrixWebserver(unsigned int port,
    std::string const & filesDir,
    MatrixController * controller)
    : pion::http::server(port)
    , m_filesDir(filesDir)
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
        
    // Initialize MIME-types
    MimeTypes::initialize();
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
    
    // Build the file path.  Special case: if the root resource was requested,
    // attempt to serve "index.html".
    std::string resource = httpRequest->get_resource();
    if ("/" == resource)
    {
        resource = "/index.html";
    }
    std::string const filePath = m_filesDir + resource;
    
    std::string statusMessage;
    unsigned int statusCode = FileServing::serveFile(filePath, writer,
        statusMessage);
        
    if (pion::http::types::RESPONSE_CODE_NOT_FOUND == statusCode)
    {
        handle_not_found_request(httpRequest, tcpConn);
    }
    if (pion::http::types::RESPONSE_CODE_FORBIDDEN == statusCode)
    {
        handle_forbidden_request(httpRequest, tcpConn,
            "msg");
    }
    if (pion::http::types::RESPONSE_CODE_SERVER_ERROR == statusCode)
    {
        handle_server_error(httpRequest, tcpConn,
            "Failed to access file");
    }
    else
    {
        r.set_status_code(statusCode);
        r.set_status_message(statusMessage);
        writer->send();
    }
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
                
            if (settingIter != params.end())
            {
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
                    // Get the setting for the given index and its type
                    MatrixSetting * setting = settingList.at(settingInt);
                    std::string type = setting->getType();
                    
                    // Check for a parameter corresponding to the setting type
                    pion::ihash_multimap::const_iterator valueIter =
                        params.find(type);
                    if (valueIter != params.end())
                    {
                        bool error = false;
                        
                        // Parse the value parameter and apply it to the setting
                        std::string const valueStr = valueIter->second;
                        std::istringstream iss(valueStr);
                        switch (setting->getID())
                        {
                            case MatrixSetting::MATRIX_SETTING_ID_STRING:
                            {
                                MatrixSettingString * const settingString =
                                    (MatrixSettingString *)setting;
                                settingString->setString(valueStr);
                                break;
                            }
                            case MatrixSetting::MATRIX_SETTING_ID_RANGED_DOUBLE:
                            {
                                MatrixSettingRangedDouble * const settingRangedDouble =
                                    (MatrixSettingRangedDouble *)setting;
                                double valueDouble;
                                iss >> valueDouble;
                                if ((iss.rdstate() & std::istringstream::failbit) != 0)
                                {
                                    error = true;
                                }
                                else
                                {
                                    settingRangedDouble->setValue(valueDouble);
                                }
                                break;
                            }
                            
                            // No default case to preserve compiler warnings for
                            // unhandled enum values
                            case MatrixSetting::MATRIX_SETTING_ID_COUNT:
                            {
                                error = true;
                                break;
                            }
                        }
                        
                        if (error)
                        {
                            writer->write("Unable to parse value into " + type
                                + ": \"" + valueStr + "\".");
                        }
                        else
                        {
                            // Write the getSettings JSON response (setSetting
                            // has the same response as getSettings).
                            r.set_content_type("application/json");
                            writer->write(
                                JSONUtils::getSettings(mode, modeInt));
                            
                            r.set_status_code(
                                pion::http::types::RESPONSE_CODE_OK);
                            r.set_status_message(
                                pion::http::types::RESPONSE_MESSAGE_OK);
                        }
                        
                    }
                    else
                    {
                        writer->write("Setting " + settingStr + " for mode "
                            + modeStr + " is of type " + type + " but no \""
                            + type + "\" parameter was given.");
                    }
                }
            }
            else
            {
                writer->write("No setting parameter given.");
            }
        }
    }
    else
    {
        writer->write("No mode parameter given.");
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