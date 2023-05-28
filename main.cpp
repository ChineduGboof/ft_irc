/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 16:23:29 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/28 17:46:48 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

//server args#     ./ircserv 6667 password
int main(int argc, char *argv[])
{
    try {

        if (argc != 3)
            throw std::runtime_error("retry: ./ircserv <port> <password>");
        (void)argv;
        return (0);
    }
    catch (std::exception &e) {
        std::cout << RED << e.what() << DEFAULT << std::endl;
    }
}