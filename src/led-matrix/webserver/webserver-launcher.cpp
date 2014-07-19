#include <led-matrix/webserver/webserver-launcher.hpp>

#include <led-matrix/webserver/webserver.hpp>

static MatrixWebserver * server = NULL;

void launchWebserver(unsigned int port, std::string const & filesDir,
    MatrixController * controller)
{
    if (NULL == server)
    {
        server = new MatrixWebserver(port, filesDir, controller);
        server->start();
    }
}

void stopWebserver()
{
    if (NULL != server)
    {
        server->stop();
        delete server;
    }
}
