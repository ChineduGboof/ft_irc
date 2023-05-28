#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
/*
To connect to Google using the resolved addresses, you can establish a 
TCP connection to one of the addresses on the specified port/service.
*/
int main() {
    const char* node = "www.google.com";  // Hostname to resolve or an ip address
    const char* service = "https";        // Service name or port number

    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(node, service, &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(result) << std::endl;
        return 1;
    }

    // Iterate over the resolved addresses and attempt to connect
    int sockfd = -1;
    struct addrinfo* addr;
    for (addr = res; addr != nullptr; addr = addr->ai_next) {
        sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sockfd == -1) {
            std::cerr << "socket error" << std::endl;
            continue;
        }

        if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) == -1) {
            std::cerr << "connect error" << std::endl;
            close(sockfd);
            sockfd = -1;
            continue;
        }

        // Connection successful
        break;
    }

    // Check if connection was established
    if (sockfd == -1) {
        std::cerr << "Failed to establish a connection" << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    // Connection established
    std::cout << "Connected to Google successfully!" << std::endl;

    // Send random data to Google
    const char* requestData = "Hello, Google!";
    int bytesSent = send(sockfd, requestData, strlen(requestData), 0);
    if (bytesSent == -1) {
        std::cerr << "send error" << std::endl;
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Receive response from Google
    const int bufferSize = 1024;
    char buffer[bufferSize];
    int bytesRead = recv(sockfd, buffer, bufferSize - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "recv error" << std::endl;
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Null-terminate the received data
    buffer[bytesRead] = '\0';

    // Print the received data
    std::cout << "Received data from Google: " << buffer << std::endl;


    // Cleanup and close the socket
    freeaddrinfo(res);
    close(sockfd);

    return 0;
}

