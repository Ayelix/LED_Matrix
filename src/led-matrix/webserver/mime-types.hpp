/* Mime-types - functions to retrieve the MIME-type for a given file extension.
 * Uses the /etc/mime.types file.
 */
#ifndef _MIME_TYPES_H
#define _MIME_TYPES_H

#include <string>

namespace MimeTypes
{
    // Initialize the MIME type list.  Should be called at startup because it
    // may take some time (1-2s) to complete.
    void initialize();
    
    // Get the MIME type for a file extension.  Returns an empty string if no
    // corresponding MIME type is found.
    std::string getMimeType(std::string const & extension);
};

#endif