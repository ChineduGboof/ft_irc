/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/30 20:51:27 by cegbulef         ###   ########.fr       */
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

		// Authenticate the password here
		// if (!authenticatePassword(password)) {
		// 	throw std::runtime_error("Server: Invalid password");
		// }

		initPollFD(_sockfd);

		while (true) {
			int pollResult = poll(_pollFD.data(), _pollFD.size(), -1);
			if (pollResult < 0) {
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
						// Password has already been authenticated, so no need to authenticate again here
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
							continue;
						}
						std::cout << "pollserver: new connection from " << remoteIP << " on socket " << fd << std::endl;
					} else {
						// Handle the data from existing clients here
						// Receive and process messages from clients
						// char buffer[1024];
						// int bytesRead = recv(_pollFD[i].fd, buffer, sizeof(buffer), 0);
						// if (bytesRead <= 0) {
						// 	// Error or connection closed
						// 	if (bytesRead < 0) {
						// 		perror("recv error");
						// 	}
						// 	// Close the client socket and remove from poll list
						// 	close(_pollFD[i].fd);
						// 	_pollFD.erase(_pollFD.begin() + i);
						// 	i--;
						// } else {
						// 	// Process the received message
						// 	std::string message(buffer, bytesRead);
						// 	// Process the message as needed
						// 	processMessage(message);
						// }
					}
				} else if (_pollFD[i].revents & POLLHUP) {
					// Client socket closed
					close(_pollFD[i].fd);
					_pollFD.erase(_pollFD.begin() + i);
					i--;
				}
			}
		}
	}


} // namespace irc
