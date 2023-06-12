/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/06/12 22:25:23 by cegbulef         ###   ########.fr       */
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
#include <algorithm>
#include "../Channels/Channel.hpp"
#include "user.hpp"
#include "Utils.hpp"
#include "Responses.hpp"

/*
You can pass these as pointers or references
        void deleteChannel(Channel* channel);
        std::vector<Channel*>& getChannels();
*/

class Channel;
class User;

namespace irc {
    class Server {
    private:
        Server();
        void initPollFD(int fd);
        void printNewConnectionInfo(const struct sockaddr_storage& remoteAddress, int fd);
        void handleSignal(int signal);
        void closeClientSocket(size_t index);
        void closeSocketAndRemoveUser(size_t index);

        std::string _host;
        int _port;
        int _status;
        int _sockfd;
        std::string _password;
        std::vector<pollfd> _pollFD;
        bool _running;

        std::vector<Channel*> _channels;
        std::vector<User*> _users;

    public:
        Server(const std::string& host, const int& port, const std::string& password);
        ~Server();

        static Server* serverInstance;
        
        void config();
        void run();
        void handleNewConnection();
        void handleClientData(size_t index);
        static void signalHandler(int signal);
        
        void createNewUser(int fd);
        void removeUser(int fd);
        std::vector<User*>& getUser();
        void sendMsg(int fd, std::string msg);
        bool authenticate_user(int index);
        std::string ExtractFromMessage(const std::string& message, const std::string& to_find);
        bool check_duplicate(std::string nick);
        int getFdByNick(std::string nick);
        
        void bye();
        bool verifyPassword(std::string userPassword);
        
        Channel* createChannel(std::string name);
        void deleteChannel(Channel channel);
        std::vector<Channel*> getChannels();
        void polling();
        void searchingForConnections();
    };
}  // namespace irc


#endif