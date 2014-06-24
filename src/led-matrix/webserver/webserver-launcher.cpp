#include <led-matrix/webserver/webserver-launcher.hpp>

#include <led-matrix/webserver/webserver.hpp>

static MatrixWebserver server;

void launchWebserver(unsigned int port)
{
    server.set_port(port);
    server.start();
}

void stopWebserver()
{
    server.stop();
}
