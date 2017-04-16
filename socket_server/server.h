//
// Standard socket server header files and main class description
// Created by Rodel Miguel on 14/4/17.
//

#ifndef HELLOTEMPLATE_SERVER_H
#define HELLOTEMPLATE_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

class Server {
public:
    enum Status
    {
        SERVER_OK = 0,
        SERVER_WAIT_CONN,
        SERVER_PORT_ERROR,
        SERVER_SOCK_ERROR,
        SERVER_SOPT_ERROR,
        SERVER_BIND_ERROR,
        SERVER_LSTN_ERROR,
        SERVER_ACPT_ERROR,
        SERVER_CONN_HNDL_ERROR,
        SERVER_STATUS_SZ
    };

    std::string status_str[SERVER_STATUS_SZ] = {
            "Server Okay",
            "Server Port Error",
            "Server Socket Creation Error",
            "Server Socket Options Error",
            "Server Bind Error",
            "Server Listen Error",
            "Server Accept Error",
            "Connection Handler Creation Error"
    };

    Server(int);
    ~Server(void);
    int Start(void);

private:

    int port;
    int status;
    int server_fd;
    struct sockaddr_in address;
    int enable;
    const int NUM_CONNECTIONS = 200;
};

void *ConnectionHandler(void *);

#endif //HELLOTEMPLATE_SERVER_H
