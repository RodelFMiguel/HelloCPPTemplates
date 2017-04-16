//
// Created by Rodel Miguel on 16/4/17.
//

#include <iostream>
#include "client.h"

void usage(char *prog)
{
    std::cerr << "Usage: " << prog << " <server address> <port>" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    std::string server = argv[1];
    int port;
    try {
        port = std::stoi(argv[2]);
    } catch (std::exception &e) {
        usage(argv[0]);
        return 1;
    }

    Client *client = nullptr;
    try {
        client = new Client(server, port);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        delete client;
        client = nullptr;
    }

    if (client != nullptr) {
        int client_fd = client->Start();
        if (client_fd <= 0) {
            std::cerr << "Client not intitialized." << std::endl;
            delete client;
            exit(0);
        }

        std::string message = "Hello Server! I'm Client.";
        unsigned char buffer[512] = {};
        int status;

        status = client->SendMsg((unsigned char*)(message.c_str()), (int)(message.length()));
        if (status <= 0) {
            std::cerr << "Client sending message error." << std::endl;
            delete client;
            exit(0);
        } else {
            std::cout << "Message Sent: " << message << std::endl;
        }

        status = client->RecvMsg(buffer, 512);
        if (status <= 0) {
            std::cerr << "Client receiving message error." << std::endl;
            delete client;
            exit(0);
        } else {
            std::cout << "Message Received: " << buffer << std::endl;
        }
    }
}
