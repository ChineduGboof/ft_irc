/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:02 by Omar              #+#    #+#             */
/*   Updated: 2023/06/04 22:03:00 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Server/Server.hpp"
#include "../Server/user.hpp"
#include <algorithm>
#include <map>

class Channel
{
	private:
		std::string name;
		std::vector<User> users;;
	public:
		Channel(std::string name);
		~Channel();
		void				addUser(User user);
		bool				operator==(Channel const &rhs) const;
		void				removeUser(User user);
		std::string			getName();
		std::vector<User>	getUsers();
		void				sendMessage(std::string message);
		void				execMessage(std::string message, User user);
		void				setMode(char mode);
};

bool	findString(std::string str, std::vector<std::string> vec);
void	joinChannel(Channel channel, User user);
void	partChannel(Channel channel, User user);

#endif