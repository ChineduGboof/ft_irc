/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/30 18:29:38 by cegbulef         ###   ########.fr       */
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
        
        struct sockaddr_in address;
        std::memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(static_cast<uint16_t>(_port));
        socklen_t addressLen = sizeof(address);

        if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("Server: socket error");
        }

        int optval = 1;
        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval)) < 0) {
            throw std::runtime_error("Server: socket options error");
        }

        if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("Server: file control error");
        }

        if (bind(_sockfd, reinterpret_cast<struct sockaddr*>(&address), addressLen) < 0) {
            throw std::runtime_error("Server: bind error");
        }

        if (listen(_sockfd, std::numeric_limits<int>::max()) < 0) {
            throw std::runtime_error("Server: listen error");
        }

        _status = ONLINE;
    }

    void Server::initPollFD(int fd) {
        struct pollfd pFD = { .fd = fd, .events = POLLIN, .revents = 0 };
        _pollFD.push_back(pFD);

        if (_pollFD.back().fd == -1) {
            throw std::runtime_error("Failed to initialize pollfd struct");
        }
    }


    void Server::run() {
        std::cout << YELLOW << "Server Run Called" << DEFAULT << std::endl;
        if (!_status)
            throw std::runtime_error("Server: Offline, must init first");

        initPollFD(_sockfd);

        while (true) {
            int pollResult = poll(_pollFD.data(), _pollFD.size(), -1);
            if (pollResult < 0) {
                perror("poll error");
                throw std::runtime_error("Server: poll error");
            }
            std::cout << "poll() result: " << pollResult << std::endl;

            for (size_t i = 0; i < _pollFD.size(); i++) {
                // Something is in the IN Queue
                if (_pollFD[i].revents & POLLIN) {
                    // We have a new connection, create a new socket for comms
                    if (_pollFD[i].fd == _sockfd) {
                        int fd = -1;
                        int addressLen = 0;
                        struct sockaddr_in address;
                        std::memset(&address, 0, sizeof(address));
                        std::cout << "HERE 5" << std::endl;
                        if ((fd = accept(_sockfd, (struct sockaddr *)&address, (socklen_t *)&addressLen)) < 0) {
                            throw std::runtime_error("Server: client connection error");
                        }
                        // Handle the new connection here (e.g., create a new client object, store the new socket)
                        initPollFD(fd);
                    }
                    else { /*Handle the data from existing clients here*/ }
                }
                else if (_pollFD[i].revents & POLLHUP) {
                    // Client socket closed
                    close(_pollFD[i].fd);
                    _pollFD.erase(_pollFD.begin() + i);
                    i--;
                }
            }
        }
    }

} // namespace irc
