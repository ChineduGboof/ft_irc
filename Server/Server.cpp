/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/29 14:15:53 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc {


    Server::Server(const std::string& host, const int& port, const std::string& password) : _host(host), _port(port) {
        std::cout << YELLOW << "Parameter Constructor Called" << DEFAULT << std::endl;
        (void)password;
    }

    Server::~Server() {}

    void Server::config() {
        std::cout << YELLOW << "Config Called" << DEFAULT << std::endl;
        struct sockaddr_in address;
        std::memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(static_cast<uint16_t>(_port));
        socklen_t addressLen = sizeof(address);
        
        // Create a socket for the server
        if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("Server: socket error");
        }
        else{std::cout << GREEN << "Socket Created" << DEFAULT << std::endl;}
        
        // Set the SO_REUSEADDR socket option to allow reusing the address, especially in fast restarts
        int optval = 1;
        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval)) < 0) {
            throw std::runtime_error("Server: socket options error");
        }
        else{std::cout << GREEN << "REUSEADDR set" << DEFAULT << std::endl;}

        // Set the socket to non-blocking mode using O_NONBLOCK flag
        // This allows the server to handle multiple clients simultaneously
        // without being blocked by any individual client's I/O operations
        if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("Server: file control error");
        }
        else{std::cout << GREEN << "O_NONBLOCK set" << DEFAULT << std::endl;}

        // Bind the socket to a specific address
        // This is necessary before the socket can start listening for incoming connections
        if (bind(_sockfd, reinterpret_cast<struct sockaddr*>(&address), addressLen) < 0) {
            throw std::runtime_error("Server: bind error");
        }
        else{std::cout << GREEN << "Binding Done" << DEFAULT << std::endl;}

        // Start listening for incoming connections on the socket
        // The maximum number of pending connections is set to the system's defined maximum
        if (listen(_sockfd, std::numeric_limits<int>::max()) < 0) {
            throw std::runtime_error("Server: listen error");
        }
        else{std::cout << GREEN << "Listening" << DEFAULT << std::endl;}
        
        _status = ONLINE;
    }

} // namespace irc
