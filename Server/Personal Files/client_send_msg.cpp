#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(sockfd);
        return 1;
    }

    // Message to send
    std::string message = "Hello, server, this is Gboof!";

    // Send the message
    ssize_t bytesSent = send(sockfd, message.c_str(), message.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Failed to send the message." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Message sent to the server." << std::endl;

    // Close the socket
    close(sockfd);

    return 0;
}
