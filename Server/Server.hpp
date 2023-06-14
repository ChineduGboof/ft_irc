/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonamog2 <yonamog2@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/06/14 11:44:02 by yonamog2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */





#ifndef SERVER_HPP
#define SERVER_HPP

#define DEFAULT		"\033[0m"
#define RED			"\033[0;31m"
#define GREEN		"\033[0;32m"
#define YELLOW		"\033[0;33m"

#include <iostream>
#include <stdexcept>
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
#include <csignal>
#include <cctype>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <limits>
#include <string>
#include <cerrno>
#include "../Channels/Channel.hpp"
#include <algorithm>
#include "user.hpp"
#include "Utils.hpp"
#include "Responses.hpp"
#include <sys/socket.h>


class Channel;
class User;
namespace irc {

    class Server {
        private:

            std::string         _host;
            int                 _port;
            int                 _status;
            int                 _sockfd;
            std::string         _password;
            std::vector<pollfd> _pollFD;
            bool                _running;
            void initPollFD(int fd);
			
        public:
            Server();
            Server( const std::string& host, const int& port, const std::string& password );
            ~Server();

            static Server*      serverInstance;
            void config();
            void run();
            void handleNewConnection();
            void handleClientData(size_t index);
            void closeClientSocket(size_t index);
            void handleSignal(int signal);
            static void signalHandler(int signal);
            void printNewConnectionInfo(const struct sockaddr_storage& remoteAddress, int fd);
            
            //user
            std::vector<User *> _users;
            void createNewUser(int fd);
            void removeUser(int fd);
            // User &getUser(int fd);
            std::vector<User *>& getUser( void );
            void sendMsg(int fd, std::string msg);
            bool authenticate_user(int index);
            std::string ExtractFromMessage(const std::string& message, const std::string &to_find);
            bool check_duplicate(std::string nick);
            int getFdByNick(std::string nick);
            bool findCap(int index);
            void displayUsers();
            void displayChannels();
            // std::string find(std::string, )

            void bye();
            bool verifyPassword(std::string userPassword);
			
			// Channels
			std::vector<Channel *> _channels;
			Channel *createChannel(std::string name);
			void deleteChannel(Channel channel);
			std::vector<Channel *> getChannels();
            void polling();
            void searchingForConnections();
            void closeSocketAndRemoveUser(size_t index);
    };
}

#endif