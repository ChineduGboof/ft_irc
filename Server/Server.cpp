/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/06/06 20:11:39 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc {

	Server* Server::serverInstance = NULL;

	Server::Server(const std::string& host, const int& port, const std::string& password) 
	: _host(host), _port(port), _password(password), _running(false) {
		serverInstance = this;  // Set the serverInstance pointer to the current instance
        std::cout << GREEN "Server: " + _host + " port: " << _port << " password: " + _password + DEFAULT<< std::flush << std::endl;
	}

	Server::~Server() {
        bye();
        std::cout << YELLOW "Hope You Enjoyed Our IRCServer 🖥️" << DEFAULT << std::endl;
    }

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

        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const char*>(&optval), sizeof(optval)) < 0) {
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
        std::cout << GREEN "Server: Listening on host:  " + _host + " port: " << _port << DEFAULT<< std::flush << std::endl;
    }

    

	void Server::initPollFD(int fd) {
		struct pollfd pFD = { 
                                .fd = fd, 
                                .events = POLLIN | POLLOUT, 
                                .revents = 0 
                            };
		_pollFD.push_back(pFD);

		if (_pollFD.back().fd == -1) {
			throw std::runtime_error("Failed to initialize pollfd struct");
		}
	}


    void Server::run() {
        if (!_status)
            throw std::runtime_error("Server: Offline, must init first");

        std::signal(SIGINT, signalHandler);
        _running = true;
        initPollFD(_sockfd);

        while (_running) {
            int pollResult = poll(_pollFD.data(), _pollFD.size(), -1);
            if (pollResult < 0) {
                if (errno == EINTR) {
                    // Signal interrupted the poll, handle it gracefully
                    handleSignal(SIGINT);
                    continue;
                }
                perror("poll error");
                throw std::runtime_error("Server: poll error");
            }
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
                } 
                
                // else if (_pollFD[i].revents & POLLOUT && _pollFD[i].fd != _sockfd) {
                //     // Something in the OUT Queue needs to be sent
                //     for ( std::deque<std::string>::iterator it = _users[i - 1]->getMsg().begin(); it != _users[i - 1]->getMsg().end(); ++it) {
                //         send(_pollFD[i].fd, it->c_str(), it->size(), MSG_NOSIGNAL);
                //         if (!_users[i - 1]->getMsg().empty())
                //             _users[i - 1]->getMsg().pop_front();
                //     }
                // } 
                
                else if (_pollFD[i].revents & POLLHUP) {
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
    }


    void Server::handleNewConnection() {
        int fd = -1;
        socklen_t addressLen = sizeof(struct sockaddr_storage);
        struct sockaddr_storage remoteAddress;
        std::memset(&remoteAddress, 0, sizeof(remoteAddress));

        if ((fd = accept(_sockfd, (struct sockaddr *)&remoteAddress, &addressLen)) < 0) {
            throw std::runtime_error("Server: client connection error");
        }

        createNewUser(fd);
        initPollFD(fd);
        printNewConnectionInfo(remoteAddress, fd);
    }
    
    // User& Server::getUser(int fd){
    //     for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++) {
    //         if (it->getUserFd() == fd) {
    //             return *it;
    //         }
    //     }
    //     return _users[0];
    // }

    void Server::handleClientData(size_t index) {
        if ( _pollFD[index].fd != _sockfd ) {
            int bytesRead = _users[index - 1]->receive();

            if ( bytesRead <= 0 ) {
                perror("recv error");
                close(_pollFD[index].fd);
                _pollFD.erase(_pollFD.begin() + index);
            }
            // m_clientCommand->cmdExecute( _users[index - 1] );
        }
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

    void Server::handleSignal(int signal) {
        std::cout << "Received signal: " << signal << std::endl;
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

    void Server::createNewUser(int fd) {
        _users.push_back( new User(fd) );
    }

    void Server::printNewConnectionInfo(const struct sockaddr_storage& remoteAddress, int fd) {
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

    // function to send messages to specific clients when needed.
    // void Server::sendToClient(size_t index, const std::string& message) {
    //     send(_pollFD[index].fd, message.c_str(), message.size(), 0);
    // }
    
} // namespace irc
