//
// Created by Rodel Miguel on 14/4/17.
//

#include <iostream>
#include "server.h"

void usage(char *prog)
{
    std::cerr << "Usage: " << prog << " <port>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    int port;
    try {
        port = std::stoi(argv[1]);
    } catch (std::exception &e) {
        usage(argv[0]);
        return 1;
    }

    Server* server = nullptr;
    try {
        server = new Server(port);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        delete server;
        server = nullptr;
    }

    if (server != nullptr) {
        if (server->Start()) {
            delete server;
            server = nullptr;
        }
    }
}
