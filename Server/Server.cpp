/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonamog2 <yonamog2@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 12:20:52 by gboof             #+#    #+#             */
/*   Updated: 2023/06/15 17:09:37 by yonamog2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc
{

	Server *Server::serverInstance = NULL;

	Server::Server() {}

	Server::Server(const std::string &host, const int &port, const std::string &password)
		: _host(host), _port(port), _password(password), _running(false)
	{
		serverInstance = this;
		std::cout << GREEN "Server: " + _host + " port: " << _port << " password: " + _password + DEFAULT << std::flush << std::endl;
		std::signal(SIGINT, signalHandler);
		config();
		run();
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

			if (!_running)
				break;
		}
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

	void Server::createNewUser(int fd)
	{
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
		_remoteIP = remoteIP;
		std::cout << YELLOW << "pollserver: new connection from " << remoteIP << " on socket " << fd << DEFAULT << std::endl;
	}

	std::string Server::getRemoteIP() const {
        return _remoteIP;
    }

	void Server::sendMsg(int fd, std::string msg)
	{
		if (send(fd, msg.c_str(), msg.length(), 0) < 0){
			throw std::runtime_error("Send error"); 
		}
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
	bool Server::check_duplicate(std::string nick)
	{
		std::vector<User *>::iterator it = _users.begin();
		while (it != _users.end())
		{
			if((*it)->getNickName() == nick)
				return (true);
			it++;
		}
		return (false);
	}
	std::string Server::scanMsg(User * users, std::string str)
	{
		for (std::vector<std::string>::const_iterator it = users->_incomingMsgs.begin(); it != users->_incomingMsgs.end(); ++it)
		{
			if(*it == str)
				if(++it != users->_incomingMsgs.end())
					return(*it);
		}
		return "";
	}
	bool Server::authenticate_user(int index)
	{
		;
		std::string pass = scanMsg(_users[index - 1], "PASS");
		std::string user_name = scanMsg(_users[index - 1], "USER");
		std::string nick_name = scanMsg(_users[index - 1], "NICK");
		// std::string pass = ExtractFromMessage(_users[index - 1]->_dataBuffer, "PASS ");
		// std::string user_name = ExtractFromMessage(_users[index - 1]->_dataBuffer, "USER ");
		// std::string nick_name = ExtractFromMessage(_users[index - 1]->_dataBuffer, "NICK ");
		std::cout << "pass: " << pass << std::endl;
		std::cout << "user: " << user_name << std::endl;
		std::cout << "nick: " << nick_name << std::endl;
		if(pass == _password && check_duplicate(nick_name) == false)
		{
			std::cout << "WELCOME : " << nick_name << " To Our IRC server , Enjoy!" << std::endl;
			// size_t x = 0;
			_users[index - 1]->setNickName(nick_name);
			_users[index - 1]->setUserName(user_name);
			_users[index - 1]->setIsAuth(true);
			// while (x < _users.size())
			// {
			// 	std::cout << "-----------------------------------------------------------------------------\n";
			// 	std::cout << "\t\t\tUser Info" << std::endl;
			// 	std::cout << "Fd:\t" << _users.at(x)->getUserFd() << std::endl;
			// 	std::cout << "Nick:\t" << _users.at(x)->getNickName() << std::endl;
			// 	std::cout << "User:\t" << _users.at(x)->getUserName() << std::endl;
			// 	std::cout << "Is_auth:\t" << _users.at(x)->getIsAuth() << std::endl;
			// 	std::cout << "Users:\t" << _users.size() << std::endl << std::endl << std::endl;
			// 	x++;
			// 	std::cout << "-----------------------------------------------------------------------------\n";
			// }
			return true;

        }
        else
        {
            if(pass == "" || pass != _password)
            {
                // std::string store = ("464 : INCORRECT PASSWORD \r\n");
                // this->sendMsg(_users[index - 1]->getUserFd(), store);
                throw std::runtime_error("INCORRECT PASSWORD\n");
            }
            if(check_duplicate(nick_name) == true)
            {
                // this->sendMsg(_users[index - 1]->getUserFd(), ("Nickname " + nick_name + " is already in use.\r\n"));
                throw std::runtime_error("duplicate user\n");
            }
            // std::cout << "-----------------------------------------------------------------------------\n";
            // std::cout << "duplicate or incorrect pass" << std::endl;
            // std::cout << "pass= " << ExtractFromMessage(_users[index - 1]->_dataBuffer, "PASS ") << std::endl;
            // std::cout << "nick= " << ExtractFromMessage(_users[index - 1]->_dataBuffer, "NICK ") << std::endl;
            // std::cout << "-----------------------------------------------------------------------------\n";
            // if(!pass.empty() || !nick_name.empty())
            //     removeUser(_users[index - 1]->getUserFd());
        }
        return false;
    }

	std::string Server::ExtractFromMessage(const std::string& message, const std::string &to_find) {
		// Find the line starting with "PASS"
			size_t lineStart = message.find(to_find);
			
			// If "PASS" line found, extract the password
			if (lineStart != std::string::npos) {
				size_t toFindStart = lineStart + std::string(to_find).length();
				
				// Find the end of the password
				size_t passwordEnd = message.find_first_of("\r\n", toFindStart);
				
				// Extract the password substring
				std::string password = message.substr(toFindStart, passwordEnd - toFindStart);
				
				return password;
			}
			
			// If no "PASS" line found, return an empty string or handle the case accordingly
			return "";
	}
	// int Server::getFdByNick(std::string nick)
	// {
	//     std::vector<User *>::iterator it = _users.begin();
	//     while (it != _users.end())
	//     {
	//         if((*it)->getUserNick() == nick)
	//             return ((*it)->getNickName());
	//         it++;
	//     }
	//     return "";
	// }
	int Server::getFdByNick(std::string nick)
	{
		std::vector<User *>::iterator it = _users.begin();
		while (it != _users.end())
		{
			if((*it)->getNickName() == nick)
				return ((*it)->getUserFd());
			it++;
		}
		return -1;
	}

    bool Server::findCap(int index)
    {
        for (std::vector<std::string>::const_iterator it = _users[index - 1]->_incomingMsgs.begin(); it != _users[index - 1]->_incomingMsgs.end(); ++it) {
            // std::cout << "Incoming Message => " << *it << std::endl;
            if(*it == "CAP")
                return true;
        }
        return false;
    }
	void Server::splitChannelInp(int index)
	{
		size_t i = 0;
		// std::cout << "size of msg : " << _users[index - 1]->_incomingMsgs.size() << std::endl;
		while (i < _users[index - 1]->_incomingMsgs.size())
		{
			if(i == 1)
			{
				std::vector<std::string> splitWords = utils::splitByDelimiter(_users[index - 1]->_incomingMsgs.at(i) , ",");
				for (std::vector<std::string>::size_type i = 0; i < splitWords.size(); ++i) 
				{
						_users[index -1 ]->_channelToJoin.push_back(splitWords[i]);
				}
			}
			if(i == 2)
			{
				std::vector<std::string> splitWords = utils::splitByDelimiter(_users[index - 1]->_incomingMsgs.at(i) , ",");
				for (std::vector<std::string>::size_type i = 0; i < splitWords.size(); ++i) 
				{
						_users[index -1 ]->_channelKeys.push_back(splitWords[i]);
				}
			}
			// std::cout << "msg: " << _users[index - 1]->_incomingMsgs.at(i) << std::endl;
			i++;
		}
		i = 0;
		while (i < _users[index -1 ]->_channelToJoin.size())
		{
			std::cout << "channels: " << _users[index -1 ]->_channelToJoin.at(i) << std::endl;
			i++;
		}
		i = 0;
		while (i < _users[index -1 ]->_channelKeys.size())
		{
			std::cout << "key: " << _users[index -1 ]->_channelKeys.at(i) << std::endl;
			i++;
		}
		return ;
	}
    void Server::handleClientData(size_t index)
    {
        if (_pollFD[index].fd != _sockfd)
        {
            int bytesRead = _users[index - 1]->receive();
            if (bytesRead <= 0)
            {
				std::cerr << "recv error" << std::endl;
                closeSocketAndRemoveUser(index);
            }
            if(findCap(index) == true && scanMsg(_users[index - 1], "PASS") == "")
            {
				// std::cout << "yoniiiiii" << std::endl;
                // std::cout << "ops got smtn:|" << _users[index - 1]->_dataBuffer << "|" << std::endl;
                return ;
            }
            if (_users[index - 1]->getIsAuth() == false)
            {
				if(_users[index - 1]->_incomingMsgs.at(0) == "JOIN" && _users[index - 1]->_incomingMsgs.size() == 2)
					return ;
                // _users.at(0)->printIncomingMsgs();
             try{
                  if(authenticate_user(index))
                {
                        std::string msg = ":irc 001 " + _users[index - 1]->getNickName() + " :Welcome to the perfect Chat system " + _users[index - 1]->getNickName() + "\n";
                        this->sendMsg(_users[index - 1]->getUserFd(), msg);
						msg = ":irc 002 " + _users[index - 1]->getNickName() + " :Host are Omar, Chinedu and Yonatan\n";
                        this->sendMsg(_users[index - 1]->getUserFd(), msg);
						msg = ":irc 003 " + _users[index - 1]->getNickName() + " :Created on july->2023\n";
                        this->sendMsg(_users[index - 1]->getUserFd(), msg);
						msg = ":irc 004 " + _users[index - 1]->getNickName()  + " :Enjoy your stay!\n";

                }
                else
                {
                        std::cout << "not authenticated\n";
                        // std::cout << "------------------------------------------------------------" << std::endl;
                        // _users[index - 1]->printIncomingMsgs();
                        // std::cout << "------------------------------------------------------------" << std::endl;
						closeSocketAndRemoveUser(index);
                }
             }
             catch(std::exception & e)
             {
				std::cout << _users[index - 1]->getNickName() + " Not Authenticated\n";
                this->sendMsg(_users[index - 1]->getUserFd(), "Error : " + std::string(e.what()));
				std::cout << RED << "Error : " + std::string(e.what()) << std::endl << DEFAULT;
				closeSocketAndRemoveUser(index);
             }
            }
            else if(_users[index - 1]->getIsAuth() == true)
            {
				if(_users[index -1 ]->_incomingMsgs.at(0) == "JOIN")
				{
					splitChannelInp(index);
					// return ;
				}
				if(_users[index - 1]->_incomingMsgs.at(0) == "QUIT")
				{
					closeSocketAndRemoveUser(index);
					return ;
				}
                //once already a memeber
				execMessage(_users[index - 1]->getMessages(), _users[index-1]); // (User, Channel
				// give me the split here so I can call execMessage
                // std::cout << "------------------------------------------------------------------------------------\n";
				// std::cout << "\t\t Incomming messages" << std::endl;
				// if(_users.size() != 0)
				// 	_users.at(0)->printIncomingMsgs();
                // std::cout << "------------------------------------------------------------------------------------\n";
                // if(_users[index - 1]->_incomingMsgs.at(0) == "PING")
                //     this->sendMsg(_users[index - 1]->getUserFd(), "PONG\r\n");
                // if(_users[index - 1]->_incomingMsgs.at(0) == "PRIVMSG" && _users[index - 1]->_incomingMsgs.at(1).at(0) != '#')
                // {
				// 	std::cout << "PRIVMSG forrrrr client " << std::endl;
                //     if(getFdByNick(_users[0]->_incomingMsgs.at(1)) != -1)
                //     {
                //  	   this->sendMsg(getFdByNick(_users[0]->_incomingMsgs.at(1)), _users[index - 1]->_incomingMsgs.at(0) + "\r\n");
                //     }
                // }
            //     std::cout << "---------------------\n";
				displayUsers();
				displayChannels();
            }
        //     // execute clieent commands
        }
    }
	void Server::displayUsers()
	{
		size_t x = 0;
		std::cout << "-----------------------------------------------------------------------------\n";
		std::cout << "\t\t\tUsers List" << std::endl;
		while (x < _users.size())
		{
			std::cout << "User_Nick:\t" << _users.at(x)->getNickName() << std::endl;
			x++;
		}
		std::cout << "-----------------------------------------------------------------------------\n";
	}
	void Server::displayChannels()
	{
		size_t x = 0;
		std::cout << "-----------------------------------------------------------------------------\n";
		std::cout << "\t\t\t Channels List" << std::endl;
		while (x < this->_channels.size())
		{
			std::cout << "channel_name:\t" << _channels.at(x)->getName() << " users : "  << _channels.at(x)->users.size() << std::endl;
			x++;
		}
		std::cout << "-----------------------------------------------------------------------------\n";
	}
	void Server::closeSocketAndRemoveUser(size_t index)
	{
		if(index  == 0)
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


	// function to send messages to specific clients when needed.
	// void Server::sendToClient(size_t index, const std::string& message) {
	//     send(_pollFD[index].fd, message.c_str(), message.size(), 0);
	// }
	std::vector<Channel *> irc::Server::getChannels()
	{
		return this->_channels;
	}

} // namespace irc

