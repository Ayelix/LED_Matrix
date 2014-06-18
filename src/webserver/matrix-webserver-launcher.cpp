#include "matrix-webserver-launcher.h"

#include "matrix-webserver.h"

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