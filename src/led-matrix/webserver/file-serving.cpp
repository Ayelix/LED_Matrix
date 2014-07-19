#include <led-matrix/webserver/file-serving.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <led-matrix/debug/debug.hpp>

namespace FileServing
{
    // Anonymous namespace for "private" items
    namespace
    {
        
    }
    
    unsigned int serveFile(std::string const & path,
        pion::http::response_writer_ptr writer,
        std::string & statusMessage)
    {
        writer->write("FileServing::serveFile(): serving file at "
            + path + '\n');
        
        // Check the given path for any ".."s or "~"s, return FORBIDDEN if there
        // are any
        if ( (path.find("..") != std::string::npos) ||
            (path.find('~') != std::string::npos))
        {
            DBG_PRINTF(
                "FileServing::serveFile(): request for illegal path \"%s\"\n",
                path.c_str());
            statusMessage = pion::http::types::RESPONSE_MESSAGE_FORBIDDEN;
            return pion::http::types::RESPONSE_CODE_FORBIDDEN;
        }
        
        // Attempt to memory-map the file
        boost::interprocess::file_mapping mapping;
        boost::interprocess::mapped_region region;
        try
        {
            mapping = boost::interprocess::file_mapping(path.c_str(),
                boost::interprocess::read_only);
            region = boost::interprocess::mapped_region(mapping,
                boost::interprocess::read_only);
        }
        catch (boost::interprocess::interprocess_exception error)
        {
            // If the file can't be mapped, assume it doesn't exist, isn't
            // accessible (permissions), or is a directory.  In any of these
            // cases, just return NOT FOUND.
            statusMessage = pion::http::types::RESPONSE_MESSAGE_NOT_FOUND;
            return pion::http::types::RESPONSE_CODE_NOT_FOUND;
        }
        DBG_PRINTF("Mapped file \"%s\" at %p with size %zu.\n",
            path.c_str(), region.get_address(), region.get_size());
        
        
        //TODO: get file MIME-type.
        //TODO: memory-map the file and write it to the writer.
        
        statusMessage = pion::http::types::RESPONSE_MESSAGE_OK;
        return pion::http::types::RESPONSE_CODE_OK;
    }
}