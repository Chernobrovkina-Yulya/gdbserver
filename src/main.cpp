#include <iostream>

#include "server.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    { 
        std::cout << "no port provided\n";
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    std::cout << "TCP server is starting\n";
    GDBServer server;
    server.StartServer(port);
    server.ProcessRequests();
    server.StopServer();
    return 0;
}