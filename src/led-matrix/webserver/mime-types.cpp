#include <led-matrix/webserver/mime-types.hpp>

#include <map>
#include <sstream>
#include <fstream>

#include <led-matrix/debug/debug.hpp>

namespace MimeTypes
{
    // Anonymous namespace for "private" items
    namespace
    {
        // The MIME-type container.  Key is the file extension, value is the
        // MIME-type.
        std::map<std::string, std::string> mimeTypes;
        
        // Flag indicating if the container has been initialized.
        bool initialized = false;
        
        // Add a MIME-type to the container
        inline void add(std::string extension, std::string mimeType)
        {
            mimeTypes[extension] = mimeType;
        }
    }
    
    void initialize()
    {
        if (!initialized)
        {
            // Open the mime.types file
            std::ifstream mimeTypesFile("/etc/mime.types");
            if (mimeTypesFile.fail())
            {
                DBG_PRINTF("MimeTypes::initialize: Failed to open /etc/mime.types.\n");
                return;
            }
            
            // Parse each line of the file
            std::string line;
            while (std::getline(mimeTypesFile, line))
            {
                std::istringstream iss(line);
                std::string extension, mimeType;
                
                // If the line contains '#', skip it
                if (std::string::npos != line.find('#'))
                {
                    continue;
                }
                
                // Get the extension and the first MIME-type from the line
                if (!(iss >> mimeType >> extension))
                {
                    // If parsing fails on one of these two, continue to the
                    // next line
                    continue;
                }
                
                // Add the first MIME-type from the line
                add(extension, mimeType);
                
                // Add any additional MIME-types from the line
                while (iss >> extension)
                {
                    add(extension, mimeType);
                }
            }
            
            initialized = true;
        }
    }
    
    std::string getMimeType(std::string const & extension)
    {
        std::string retVal = "";
        
        // Initialize MIME-type container if not already initialized
        initialize();
        
        // Find MIME-type corresponding to given extension
        std::map<std::string, std::string>::const_iterator iter =
            mimeTypes.find(extension);
        if (mimeTypes.end() != iter)
        {
            retVal = iter->second;
        }
        
        return retVal;
    }
}