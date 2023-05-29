/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:50 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/29 09:25:44 by gboof            ###   ########.fr       */
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
# include <csignal>

namespace irc {

    class Server {

        private:

            int                                 _port;
            int                                 _status;
            std::string                         _host;
            static volatile std::sig_atomic_t   _signalStatus;

            static void signalHandler(int signal);
            Server();

        public:
            Server( const std::string& host, const int& port, const std::string& password );
            ~Server();

            void config();
            bool isOnline();
            void stop();


    };
}

#endif