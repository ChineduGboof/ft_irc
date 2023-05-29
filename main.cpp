/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 16:23:29 by cegbulef          #+#    #+#             */
/*   Updated: 2023/05/29 16:17:48 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "Server/Utils.hpp"

int main(int argc, char *argv[])
{
    int         port;
    std::string password;

    try 
    {
        if (argc != 3)
            throw std::runtime_error("retry: ./ircserv <port> <password>");

        port = utils::atoi(argv[1]);
        if (port < 0 || port > 65535)
            throw std::out_of_range("Select Port number (0 -> 65535)");

        password = utils::whitespace(argv[2]);
        std::cout << "Port: " << port << " Password: " << password << std::endl;

        irc::Server server("0.0.0.0", port, password);
        server.config();
        while (true) {
            // Optionally, you can add logic here to handle incoming connections or other tasks
            // This loop will keep the program execution alive until it is terminated externally
        }
    }
    catch (std::exception &e) 
    {
        std::cout << RED << e.what() << DEFAULT << std::endl;
    }
    catch ( ... ) {
        std::cout << "Fatal Error: An unhandled exception occurred." << std::endl;
        return EXIT_FAILURE;
    }

    return (0);
}