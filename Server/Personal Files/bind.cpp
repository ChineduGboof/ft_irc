
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

/*
In this example, the code binds a socket to port 80 
(the standard HTTP port) of the IP address associated with "www.google.com".
By binding the socket to a specific IP address and port, 
you can create a server that listens for incoming connections on that address. 
*/

int main()
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Obtain address information for the desired IP address and port
    int status = getaddrinfo("www.google.com", "80", &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }

    // Create a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        std::cerr << "socket error" << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    // Bind the socket to the address
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "bind error" << std::endl;
        freeaddrinfo(res);
        close(sockfd);
        return 1;
    }

    // At this point, the socket is bound to the specified address
    std::cout << "Socket bound successfully!" << std::endl;

    // Further operations using the bound socket can be performed here

    freeaddrinfo(res);
    close(sockfd);

    return 0;
}
