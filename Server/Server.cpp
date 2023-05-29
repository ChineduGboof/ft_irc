/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/29 12:17:47 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc {


    Server::Server(const std::string& host, const int& port, const std::string& password) : _host(host), _port(port) {
        std::cout << "\033[33mParameter Constructor Called\033[0m" << std::endl;
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

        if (fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | O_NONBLOCK) < 0) {
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




} // namespace irc
