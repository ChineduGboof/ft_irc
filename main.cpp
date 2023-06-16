/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 16:23:29 by cegbulef          #+#    #+#             */
/*   Updated: 2023/06/16 18:34:01 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "Server/Utils.hpp"

int main(int argc, char *argv[])
{
    irc::Server* server = NULL;

    try
    {
        if (argc != 3)
        {
            throw std::runtime_error("retry: ./ircserv <port> <password>");
        }

        int port = utils::atoi(argv[1]);
        if (port < 0 || port > 65535)
        {
            throw std::out_of_range("Select Port number (0 -> 65535)");
        }

        std::string password = utils::whitespace(argv[2]);

        server = new irc::Server("0.0.0.0", port, password);
        irc::Server::serverInstance = server;

        std::signal(SIGINT, &irc::Server::signalHandler);
        std::signal(SIGTERM, &irc::Server::signalHandler);

        server->config();
        server->run();
    }
    catch (const std::exception& e)
    {
        std::cout << RED << e.what() << DEFAULT << std::endl;
        delete server;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Fatal Error: An unhandled exception occurred." << std::endl;
        delete server;
        return EXIT_FAILURE;
    }

    delete server;
    return EXIT_SUCCESS;
}
