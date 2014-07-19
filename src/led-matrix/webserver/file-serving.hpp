/* File-serving - Functions to handle serving of files in the webserver.
 */
#ifndef _FILE_SERVING_H
#define _FILE_SERVING_H

#include <string>

#include <pion/http/response_writer.hpp>

namespace FileServing
{
    // Attempt to serve a file at the given path using the given
    // pion::http::response_writer.  Returns a status code to indicate
    // success/error (defined in pion::http::types as RESPONSE_CODE_*) and puts
    // the corresponding status message in the 'statusMessage' reference.
    unsigned int serveFile(std::string const & path,
        pion::http::response_writer_ptr writer,
        std::string & statusMessage);
};

#endif