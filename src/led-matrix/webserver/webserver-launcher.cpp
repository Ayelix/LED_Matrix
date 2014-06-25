#include <led-matrix/webserver/webserver-launcher.hpp>

#include <led-matrix/webserver/webserver.hpp>

static MatrixWebserver * server = NULL;

void launchWebserver(unsigned int port, MatrixController * controller)
{
    if (NULL == server)
    {
        server = new MatrixWebserver(port, controller);
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
