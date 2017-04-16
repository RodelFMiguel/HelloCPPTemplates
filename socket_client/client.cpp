//
// Created by Rodel Miguel on 14/4/17.
//

#include <iostream>
#include "client.h"

Client::Client(std::string server, int port):  server_ip(server), server_port(port)
{
    if (port < 0) {
        std::cerr << "Error: port number " << port << " cannot be negative" << std::endl;
        status = CLIENT_PORT_ERROR;
    } else {
        memset(&server_address, 0, sizeof(server_address));
        std::cout << "Client created." << std::endl;
    }
}

Client::~Client(void)
{
    if (close(client_fd) != 0) {
        std::cerr << "Cannot close connection: " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "Connection closed." << std::endl;
    std::cout << "Client terminated." << std::endl;
}

int Client::Start(void)
{
    // Creating socket file descriptor
    // IPV4 = AF_INET; IPV6 = IF_INET6
    // TCP Socket = SOCK_STREAM; UDP Socket = SOCK_DGRAM
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        status = CLIENT_SOCK_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return -status;
    }

    // Setup the server address structure and the port to connect to
    // sin_family: For  AF_INET, AF_INET6, AF_UNIXm AF_APPLETALK, AF_PACKET, AF_X25, AF_NETLINK
    // sin_addr.s_addr: attach to a specific address, INADDR_ANY to bind to any address
    // sin_port: port number to bind to
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    // convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr)<=0)
    {
        status = CLIENT_ADDR_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return -status;
    }

    // Connect to the socket referred to by the file descriptor (client_fd)
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        status = CLIENT_CONN_ERROR;
        std::cerr << "Error: " << status_str[status] << " : " << strerror(errno) << std::endl;
        return -status;
    }

    return client_fd;
}

int Client::SendMsg(unsigned char *msg, int len)
{
    if (client_fd <= 0) {
        status = CLIENT_SOCK_INIT_ERROR;
        std::cerr << "Error: " << status_str[status] << std::endl;
        return -status;
    }

    if (!msg) {
        status = CLIENT_MSG_ERROR;
        std::cerr << "Error: " << status_str[status] << std::endl;
        return -status;
    }

    return send(client_fd, msg , len, 0);
}

int Client::RecvMsg(unsigned char *msg, int len)
{
    if (client_fd <= 0) {
        status = CLIENT_SOCK_INIT_ERROR;
        std::cerr << "Error: " << status_str[status] << std::endl;
        return -status;
    }

    if (!msg) {
        status = CLIENT_MSG_ERROR;
        std::cerr << "Error: " << status_str[status] << std::endl;
        return -status;
    }

    return read(client_fd, msg, len);
}
