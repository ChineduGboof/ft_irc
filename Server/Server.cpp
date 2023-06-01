/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/06/01 21:38:32 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc {

	Server* Server::serverInstance = NULL;

	Server::Server(const std::string& host, const int& port, const std::string& password) 
	: _host(host), _port(port), _password(password), _running(false) {
		std::cout << YELLOW << "Parameter Constructor Called" << DEFAULT << std::endl;
		(void)password;
		serverInstance = this;  // Set the serverInstance pointer to the current instance
	}

	Server::~Server() {}

	void Server::config() {
		
		struct sockaddr_in address;
		socklen_t addressLen = sizeof(address);
		std::memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(static_cast<uint16_t>(_port));

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

        std::signal(SIGINT, signalHandler);
        _running = true;
        initPollFD(_sockfd);

        while (_running) {
            int pollResult = poll(_pollFD.data(), _pollFD.size(), -1);
            if (pollResult < 0) {
                // Check if the poll was interrupted by a signal
                if (errno == EINTR) {
                    // Signal interrupted the poll, handle it gracefully
                    handleSignal(SIGINT);
                    continue;
                }
                perror("poll error");
                throw std::runtime_error("Server: poll error");
            }
            std::cout << "Number of Clients: " << _pollFD.size() << std::endl;

            // Run through the existing connections looking for data to read
            for (size_t i = 0; i < _pollFD.size(); i++) {
                // Check if someone's ready to read
                if (_pollFD[i].revents & POLLIN) {
                    // We have a new connection, create a new socket for comms
                    if (_pollFD[i].fd == _sockfd) {
                        handleNewConnection();
                    } else {
                        // Handle the data from existing clients here
                        handleClientData(i);
                    }
                } else if (_pollFD[i].revents & POLLHUP) {
                    // Client socket closed
                    closeClientSocket(i);
                    i--;
                }
            }
            // Check if the server should stop
            if (!_running) {
                break;
            }
        }
        // Perform any necessary cleanup before exiting the loop
        bye();
    }

    void Server::handleNewConnection() {
        int fd = -1;
        socklen_t addressLen = sizeof(struct sockaddr_storage);
        struct sockaddr_storage remoteAddress;
        std::memset(&remoteAddress, 0, sizeof(remoteAddress));

        if ((fd = accept(_sockfd, (struct sockaddr *)&remoteAddress, &addressLen)) < 0) {
            throw std::runtime_error("Server: client connection error");
        }
        // Handle the new connection here (e.g., create a new client object, store the new socket)
        initPollFD(fd);

        // Print the new connection information
        char remoteIP[INET6_ADDRSTRLEN];
        if (remoteAddress.ss_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)&remoteAddress;
            inet_ntop(AF_INET, &(ipv4->sin_addr), remoteIP, INET_ADDRSTRLEN);
        } else if (remoteAddress.ss_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&remoteAddress;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), remoteIP, INET6_ADDRSTRLEN);
        } else {
            std::cout << "Unknown address family" << std::endl;
            return;
        }
        std::cout << YELLOW << "pollserver: new connection from " << remoteIP << " on socket " << fd << DEFAULT << std::endl;
    }

    void Server::handleClientData(size_t index) {
        char buffer[1024];
        int bytesRead = recv(_pollFD[index].fd, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            // Error or connection closed
            if (bytesRead < 0) {
                perror("recv error");
            }
            // Close the client socket and remove from poll list
            close(_pollFD[index].fd);
            _pollFD.erase(_pollFD.begin() + index);
            return;
        }
        // Process the received message
        std::string message(buffer, bytesRead);
        
        // if you get a PING, respond with PONG
        // if (message.substr(0, 4) == "PING") {
        //     // Respond with PONG
        //     std::string pongMessage = "PONG" + message.substr(4);
        //     send(_pollFD[index].fd, pongMessage.c_str(), pongMessage.size(), 0);
        //     return;
        // }
        
        // Print the received message
        std::cout << GREEN << "Received message from client: " << DEFAULT << message << std::endl;
    }

    void Server::closeClientSocket(size_t index) {
        close(_pollFD[index].fd);
        _pollFD.erase(_pollFD.begin() + index);
    }

    void Server::bye() {
        if (_sockfd != -1) {
            std::cout << RED << "Server: Stopped socket closed" << DEFAULT << std::endl;
            close(_sockfd);
            _sockfd = -1;

            // Add any additional cleanup code here
        }
    }

    // Signal handling function
    void Server::handleSignal(int signal) {
        std::cout << "Received signal: " << signal << std::endl;
        // Add necessary actions to handle the signal
        // For example, you can call the stop() function here to stop the server gracefully
        bye();
        _running = false;  // Set _running to false to stop the server
    }

    void Server::signalHandler(int signal) {
        if (serverInstance != NULL) {
            serverInstance->handleSignal(signal);
        }
    }

    bool Server::verifyPassword(std::string userPassword) {
        if (userPassword == _password)
            return true;
        return false;
    }

    // function to send messages to specific clients when needed.
    // void Server::sendToClient(size_t index, const std::string& message) {
    //     send(_pollFD[index].fd, message.c_str(), message.size(), 0);
    // }
    
} // namespace irc
