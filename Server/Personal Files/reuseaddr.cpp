#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

int main() {
    int listener;
    int yes = 1;

    // Create a socket
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1) {
        perror("socket");
        exit(1);
    }

    // Set the SO_REUSEADDR option
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // Bind the socket to a specific address and port
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces
    serverAddress.sin_port = htons(8080);  // Choose the desired port

    if (bind(listener, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind");
        exit(1);
    }

    std::cout << "Server is listening on port 8080" << std::endl;

    // Close the socket
    close(listener);

    return 0;
}
