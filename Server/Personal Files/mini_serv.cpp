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
#include <fcntl.h>
#include <poll.h>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;

void handleNewConnection(int serverSocket);
void handleClientData(int clientSocket, char* buffer);
void sendIRCMessage(int clientSocket, const std::string& message);
void closeClientSocket(int clientSocket);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    char buffer[BUFFER_SIZE];

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket");
        exit(1);
    }

    // Set socket options
    int reuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // Set up the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(1);
    }

    // std::cout << "IRC server listening on port " << atoi(argv[1]) << std::endl;
    std::cout << "IRC server listening on: " << inet_ntoa(serverAddress.sin_addr) << ":" << atoi(argv[1]) << std::endl;

    // Set the server socket to non-blocking mode
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    struct pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; ++i) {
        fds[i].fd = -1;
    }

    int activeClients = 0;

    while (true) {
        // Wait for events on any of the file descriptors
        int pollResult = poll(fds, activeClients + 1, -1);
        if (pollResult == -1) {
            perror("poll");
            exit(1);
        }

        // Check if a new client connection is available
        if (fds[0].revents & POLLIN) {
            handleNewConnection(serverSocket);
            fds[0].revents = 0;
        }

        // Check for activity on client sockets
        for (int i = 1; i <= MAX_CLIENTS; ++i) {
            if (fds[i].fd != -1 && fds[i].revents != 0) {
                if (fds[i].revents & POLLIN) {
                    handleClientData(fds[i].fd, buffer);
                }

                if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    closeClientSocket(fds[i].fd);
                    fds[i].fd = -1;
                    --activeClients;
                }

                fds[i].revents = 0;
            }
        }
    }

    return 0;
}

void handleNewConnection(int serverSocket) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        perror("accept");
        return;
    }

    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        close(clientSocket);
        return;
    }

    std::cout << "New client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

    // Send the CAP LS 302 message
    sendIRCMessage(clientSocket, "CAP LS 302");

    // Create an array of pollfd structures
    std::vector<pollfd> fds(MAX_CLIENTS);
    // Add the new client socket to the list of file descriptors
    for (int i = 1; i <= MAX_CLIENTS; ++i) {
        if (fds[i].fd == -1) {
            fds[i].fd = clientSocket;
            fds[i].events = POLLIN;
            break;
        }
    }
}

void handleClientData(int clientSocket, char* buffer) {
    ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1) {
        perror("recv");
        return;
    }

    if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        closeClientSocket(clientSocket);
        return;
    }

    // Process the received data
    // ...

    // Example: Echo the received data back to the client
    if (send(clientSocket, buffer, bytesRead, 0) == -1) {
        perror("send");
        closeClientSocket(clientSocket);
        return;
    }
}

void sendIRCMessage(int clientSocket, const std::string& message) {
    std::string fullMessage = message + "\r\n";
    if (send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0) == -1) {
        perror("send");
        closeClientSocket(clientSocket);
    }
}

void closeClientSocket(int clientSocket) {
    close(clientSocket);
    std::cout << "Client socket closed" << std::endl;
}
