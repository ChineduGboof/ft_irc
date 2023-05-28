#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>


/*
test the functionality of the getaddrinfo() function in C++ 
by writing a program that utilizes it to resolve hostnames 
or service names into network addresses. 
*/

int main() {
    const char* node = "www.google.com";  // Hostname to resolve
    const char* service = "https";          // Service name or port number

    struct addrinfo hints;
    struct addrinfo* res;

    // Set hints to zero and configure the desired criteria
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    // Call getaddrinfo to resolve the hostname and service
    int result = getaddrinfo(node, service, &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(result) << std::endl;
        return 1;
    }

    // Traverse the linked list of addrinfo structures and print the resolved addresses
    for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        // Call getnameinfo to convert the network address to human-readable format
        int nameResult = getnameinfo(p->ai_addr, p->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0);
        if (nameResult != 0) {
            std::cerr << "getnameinfo error: " << gai_strerror(nameResult) << std::endl;
            continue;
        }

        std::cout << "Resolved address: " << host << " | Port/Service: " << service << std::endl;
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(res);

    return 0;
}
