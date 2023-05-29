/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:38:53 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/29 08:52:30 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

namespace irc {

    Server::Server( const std::string& host, const int& port, const std::string& password ): _host(host), _port(port) {
        std::cout << YELLOW << "Parameter Constructor Called" << DEFAULT << std::endl;
    }

    Server::~Server() {}
    
    void Server::config(){}
}