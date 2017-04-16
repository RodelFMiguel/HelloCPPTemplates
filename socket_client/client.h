//
// Created by Rodel Miguel on 14/4/17.
//

#ifndef HELLOTEMPLATE_CLIENT_H
#define HELLOTEMPLATE_CLIENT_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

class Client
{
public:
    enum Status
    {
        CLIENT_OK = 0,
        CLIENT_PORT_ERROR,
        CLIENT_SOCK_ERROR,
        CLIENT_ADDR_ERROR,
        CLIENT_CONN_ERROR,
        CLIENT_SOCK_INIT_ERROR,
        CLIENT_MSG_ERROR,
        CLIENT_STATUS_SZ
    };

    std::string status_str[CLIENT_STATUS_SZ] = {
            "Client Okay",
            "Client Port Error",
            "Client Socket Creation Error",
            "Client Invalid Address",
            "Client Connection Error",
            "Socket Not Initialized",
            "Invalid Message/Buffer"
    };

    Client(std::string, int);
    ~Client();
    int Start();
    int SendMsg(unsigned char *, int);
    int RecvMsg(unsigned char *, int);

private:
    std::string server_ip;
    int server_port;
    int client_fd;
    struct sockaddr_in server_address;
    int status;
};

#endif //HELLOTEMPLATE_CLIENT_H
