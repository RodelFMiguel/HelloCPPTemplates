//
// Standard socket server code template
// Created by Rodel Miguel on 14/4/17.
//

#include <iostream>
#include "server.h"

Server::Server(int port): port(port), status(SERVER_OK), server_fd(0), enable(1)
{
    if (port < 0) {
        std::cerr << "Error: port number " << port << " cannot be negative" << std::endl;
        status = SERVER_PORT_ERROR;
    } else {
        memset(&address, 0, sizeof(address));
        std::cout << "Server created." << std::endl;
    }
}

Server::~Server(void)
{
    if (close(server_fd) != 0) {
        std::cerr << "Cannot close connection: " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "Connection closed." << std::endl;
    std::cout << "Server terminated." << std::endl;
}

int Server::Start(void)
{
    if (status) {
        std::cerr << "Error: " << status_str[status] << std::endl;
        return status;
    }

    // Creating socket file descriptor
    // IPV4 = AF_INET; IPV6 = IF_INET6
    // TCP Socket = SOCK_STREAM; UDP Socket = SOCK_DGRAM
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        status = SERVER_SOCK_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return errno;
    }

    // Setting server socket options
    // See: https://linux.die.net/man/3/setsockopt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &enable, sizeof(int)) < 0) {
        status = SERVER_SOPT_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return errno;
    }

    // Setup the server address structure and bind the socket to that port
    // sin_family: For  AF_INET, AF_INET6, AF_UNIXm AF_APPLETALK, AF_PACKET, AF_X25, AF_NETLINK
    // sin_addr.s_addr: attach to a specific address, INADDR_ANY to bind to any address
    // sin_port: port number to bind to
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((uint16_t)port);
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        status = SERVER_BIND_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return errno;
    }

    // Start listening on the assigned port
    if (listen(server_fd, NUM_CONNECTIONS) < 0) {
        status = SERVER_LSTN_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return errno;
    } else {
        std::cout << "Listening to port " << port << std::endl;
    }

    // Start accepting connections from clients
    struct sockaddr_in client_addr;
    int client_fd;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    while (1) {
        status = SERVER_WAIT_CONN;
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
        if (client_fd < 0) {
            status = SERVER_ACPT_ERROR;
            std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
            continue;
        }

        long *data = new long[2];
        data[0] = (long)client_fd;
        data[1] = (long)this;

        std::cout << "Creating a new thread for a new connection..." << std::endl;
        if (pthread_create(&thread_id, nullptr, ConnectionHandler, (void *)data) < 0) {
            status = SERVER_CONN_HNDL_ERROR;
            std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
            continue;
        }
    }
}

void *ConnectionHandler(void *pdata)
{
    long *data = (long *)pdata;
    int client_fd = (int)data[0];
    Server *server = (Server *)data[2];
    delete[] data;
    unsigned char recv_msg[512];
    char send_msg[] = "Hello! Is this server's reply!";
    int recv_sz;

    std::cout << "ConnectionHander Initiated for " << client_fd << std::endl;

    std::cout << "Received Data: ";
    do {
        recv_sz = read(client_fd, recv_msg, 512);
        std::cout << recv_msg;
    } while (recv_sz == 512);
    std::cout << std::endl;

    send(client_fd, send_msg, strlen(send_msg) , 0);
    std::cout << "Server Message Sent: " << send_msg << std::endl;
}


