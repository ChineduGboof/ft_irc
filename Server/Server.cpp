/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 12:20:52 by gboof             #+#    #+#             */
/*   Updated: 2023/06/11 15:06:46 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

#define HOST_P "002"
namespace irc
{

    Server *Server::serverInstance = NULL;
    Server::Server() {}

    Server::Server(const std::string &host, const int &port, const std::string &password)
        : _host(host), _port(port), _password(password), _running(false)
    {
        serverInstance = this; // Set the serverInstance pointer to the current instance
        std::cout << GREEN "Server: " + _host + " port: " << _port << " password: " + _password + DEFAULT << std::flush << std::endl;
    }

    Server::~Server()
    {
        bye();
        std::cout << YELLOW "Hope You Enjoyed Our IRCServer 🖥️" << DEFAULT << std::endl;
    }

    void Server::config()
    {
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
        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&optval), sizeof(optval)) < 0) {
            throw std::runtime_error("Server: socket options error");
        }
        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const char *>(&optval), sizeof(optval)) < 0) {
            throw std::runtime_error("Server: socket options error");
        }
        if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("Server: file control error");
        }
        if (bind(_sockfd, reinterpret_cast<struct sockaddr *>(&address), addressLen) < 0) {
            throw std::runtime_error("Server: bind error");
        }
        if (listen(_sockfd, std::numeric_limits<int>::max()) < 0) {
            throw std::runtime_error("Server: listen error");
        }
        _status = ONLINE;
        std::cout << GREEN "Server: Listening on host:  " + _host + " port: " << _port << DEFAULT << std::flush << std::endl;
    }

    void Server::initPollFD(int fd)
    {
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

    void Server::sendMsg(int fd, std::string msg)
    {
        if (send(fd, msg.c_str(), msg.length(), 0) < 0){
            throw std::runtime_error("Send error"); 
        }
    }
    
    void Server::polling()
    {
        int pollResult = poll(_pollFD.data(), _pollFD.size(), -1);
        if (pollResult < 0)
        {
            if (errno == EINTR)
            {
                handleSignal(SIGINT);
                std::cerr << RED << "POLL: poll error" << DEFAULT << std::endl;
                throw std::runtime_error(" ");
            }
        }
    }

    void Server::run()
    {
        if (!_status)
            throw std::runtime_error("Server: Offline, must init first");

        std::signal(SIGINT, signalHandler);
        _running = true;
        initPollFD(_sockfd);

        while (_running)
        {
            polling();
            searchingForConnections();

            if (!_running) {
                break;
            }
        }
    }

/**
 * @brief: If someone's ready to read.
 * Check if it is a new connection and create a new socket for communicaion
 * else its an existing client, so handle the data
*/
    void Server::searchingForConnections()
    {
        for (size_t i = 0; i < _pollFD.size(); i++)
        {
            if (_pollFD[i].revents & POLLIN)
            {
                if (_pollFD[i].fd == _sockfd)
                    handleNewConnection();
                else
                    handleClientData(i);
            }
            else if (_pollFD[i].revents & POLLOUT && _pollFD[i].fd != _sockfd)
            {
                // Something in the OUT Queue needs to be sent
                for (std::deque<std::string>::iterator it = _users[i - 1]->getOutgoingMsg().begin(); it != _users[i - 1]->getOutgoingMsg().end(); ++it)
                {
                    send(_pollFD[i].fd, it->c_str(), it->size(), 0);
                    if (!_users[i - 1]->getOutgoingMsg().empty())
                        _users[i - 1]->getOutgoingMsg().pop_front();
                }
            }
            else if (_pollFD[i].revents & POLLHUP)
            {
                // Client socket closed
                closeClientSocket(i);
                i--;
            }
        }
    }

    void Server::handleNewConnection()
    {
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
    
    void Server::removeUser(int fd)
    {
        for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
        {
            if ((*it)->getUserFd() == fd)
            {
                _users.erase(it);
                break;
            }
        }
    }
    void Server::authenticate_user(int index)
    {
        // int i = 0;
        // for (std::deque<std::vector<std::string> >::iterator it = _users[index - 1]->_incomingMsgs.begin(); it != _users[index - 1]->_incomingMsgs.end(); ++it) {
        //     for (std::vector<std::string>::iterator strIt = it->begin(); strIt != it->end(); ++strIt) {
        //         std::cout << "new_get " << i++ << " => " << *strIt << std::endl;  // Print each word in a new line
        //     }
        // }
        if(ExtractFromMessage(_users[index - 1]->_dataBuffer, "PASS ") == _password)
        {
            std::cout << "correct pass\n";
        }
        else
        {
            std::cout << "incorrect password\n";
        }
        _users[index - 1]->setIsAuth(true);
    }

    std::string Server::ExtractFromMessage(const std::string& message, const std::string &to_find) {
        // Find the line starting with "PASS"
            size_t passLineStart = message.find(to_find);
            
            // If "PASS" line found, extract the password
            if (passLineStart != std::string::npos) {
                size_t passwordStart = passLineStart + std::string(to_find).length();
                
                // Find the end of the password
                size_t passwordEnd = message.find_first_of("\r\n", passwordStart);
                
                // Extract the password substring
                std::string password = message.substr(passwordStart, passwordEnd - passwordStart);
                
                return password;
            }
            
            // If no "PASS" line found, return an empty string or handle the case accordingly
            return "";
    }

void Server::handleClientData(size_t index)
{
    if (_pollFD[index].fd != _sockfd)
    {
        int bytesRead = _users[index - 1]->receive();

        if (bytesRead <= 0) {
            closeSocketAndRemoveUser(index);
        }

        std::cout << "--------------------------\n"
                  << std::endl;
        std::cout << "password: " << ExtractFromMessage(_users[index - 1]->_dataBuffer, "PASS ") << std::endl;
        std::cout << "message: " << _users[index - 1]->_dataBuffer << std::endl;
        std::cout << "\n--------------------------" << std::endl;

        if (_users[index - 1]->getIsAuth() == false)
        {
            authenticate_user(index);
            if (_users[index - 1]->getIsAuth() == true)
            {
                this->sendMsg(_users[index - 1]->getUserFd(), "001 :Welcome\r\n");
            }
            else
            {
                //do something else if it's not authenticated
                std::cout << "not authenticated\n";
            }
        }
        else
        {
            std::cout << "---------------------\n";
            std::cout << "already member\n"
                      << _users.size() << std::endl;
            std::cout << "---------------------\n";
        }
    }
}

void Server::closeSocketAndRemoveUser(size_t index)
{
    std::cerr << "recv error" << std::endl;
    close(_pollFD[index].fd);
    _pollFD.erase(_pollFD.begin() + index);
    removeUser(_users[index - 1]->getUserFd());
}


    std::vector<User *> &Server::getUser(void)
    {
        return _users;
    }

    // User& Server::getUser(int fd){
    //     for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++) {
    //         if (it->getUserFd() == fd) {
    //             return *it;
    //         }
    //     }
    //     return _users[0];
    // }

    void Server::closeClientSocket(size_t index)
    {
        close(_pollFD[index].fd);
        _pollFD.erase(_pollFD.begin() + index);
    }

    void Server::bye()
    {
        if (_sockfd != -1)
        {
            std::cout << RED << "Server: Stopped socket closed" << DEFAULT << std::endl;
            close(_sockfd);
            _sockfd = -1;

            // Add any additional cleanup code here
        }
    }

    void Server::handleSignal(int signal)
    {
        std::cout << "Received signal: " << signal << std::endl;
        _running = false; // Set _running to false to stop the server
    }

    void Server::signalHandler(int signal)
    {
        if (serverInstance != NULL)
        {
            serverInstance->handleSignal(signal);
        }
    }

    bool Server::verifyPassword(std::string userPassword)
    {
        if (userPassword == _password)
            return true;
        return false;
    }

    void Server::createNewUser(int fd)
    {
        if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("Server: file control error"); 
        }
        _users.push_back(new User(fd));
        std::string message;

        message = "CAP * ACK multi-prefix\r\n";
        send(fd, message.c_str(), message.length(), 0);
    }

    void Server::printNewConnectionInfo(const struct sockaddr_storage &remoteAddress, int fd)
    {
        char remoteIP[INET6_ADDRSTRLEN];

        if (remoteAddress.ss_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)&remoteAddress;
            inet_ntop(AF_INET, &(ipv4->sin_addr), remoteIP, INET_ADDRSTRLEN);
        }
        else if (remoteAddress.ss_family == AF_INET6)
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&remoteAddress;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), remoteIP, INET6_ADDRSTRLEN);
        }
        else
        {
            std::cout << "Unknown address family" << std::endl;
            return;
        }

        std::cout << YELLOW << "pollserver: new connection from " << remoteIP << " on socket " << fd << DEFAULT << std::endl;
    }

    // function to send messages to specific clients when needed.
    // void Server::sendToClient(size_t index, const std::string& message) {
    //     send(_pollFD[index].fd, message.c_str(), message.size(), 0);
    // }
    std::vector<Channel> irc::Server::getChannels()
	{
		return this->_channels;
	}
} // namespace irc
