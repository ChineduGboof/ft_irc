#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    int sockfd, newsockfd;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Set up the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces
    serverAddress.sin_port = htons(8080);  // Choose the desired port

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind");
        exit(1);
    }

    // Get the bound address
    struct sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    if (getsockname(sockfd, (struct sockaddr*)&boundAddress, &boundAddressLength) == -1) {
        perror("getsockname");
        exit(1);
    }

    // Convert the bound address to a string representation
    char boundIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(boundAddress.sin_addr), boundIP, INET_ADDRSTRLEN);

    // Print the bound IP address
    printf("Server is bound to IP address: %s\n", boundIP);

    // Start listening for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    std::cout << "Server is listening on port 8080" << std::endl;

    // Accept an incoming connection
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newsockfd == -1) {
        perror("accept");
        exit(1);
    }

    std::cout << "Accepted a new client connection" << std::endl;

    // Send a message to the client
    const char* message = "Hello, client!";
    int messageSize = strlen(message);

    if (send(newsockfd, message, messageSize, 0) == -1) {
        perror("send");
        exit(1);
    }

    std::cout << "Sent message to the client: " << message << std::endl;

    // Receive a response from the client
    char buffer[1024];
    int bufferSize = sizeof(buffer);
    int bytesRead = recv(newsockfd, buffer, bufferSize - 1, 0);
    if (bytesRead == -1) {
        perror("recv");
        exit(1);
    }

    buffer[bytesRead] = '\0';  // Add null terminator to received data
    std::cout << "Received response from the client: " << buffer << std::endl;

    // Close the sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
