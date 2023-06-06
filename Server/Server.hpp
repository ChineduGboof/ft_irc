/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:50 by cegbulef          #+#    #+#             */
/*   Updated: 2023/06/06 20:13:38 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define DEFAULT		"\033[0m"
#define RED			"\033[0;31m"
#define GREEN		"\033[0;32m"
#define YELLOW		"\033[0;33m"

#define ONLINE  1

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
#include <sstream>
#include <limits>
#include <string>
#include <cerrno>
#include "../Channels/Channel.hpp"
#include <algorithm>
#include "user.hpp"

class Channel;
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


            Server();
            void initPollFD(int fd);
			
			std::vector<Channel> _channels;
        public:
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
            
            void createNewUser(int fd);
            void removeUser(int fd);
            std::vector<User> users;
            void bye();

            bool verifyPassword(std::string userPassword);
			
			// Channels
			void createChannel(std::string name);
			void deleteChannel(Channel channel);
			std::vector<Channel> getChannels();
    };
}

#endif