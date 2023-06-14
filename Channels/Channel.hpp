/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonamog2 <yonamog2@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:02 by Omar              #+#    #+#             */
/*   Updated: 2023/06/14 13:27:34 by yonamog2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Server/Server.hpp"
#include "../Server/user.hpp"
#include <algorithm>
#include <map>

class User;
class Channel
{
	private:
		std::string				name;
		std::map<char, bool>	modes;
		unsigned int			maxUsers;
		std::string				topic;
	public:
		Channel(std::string name);
		~Channel();
		std::vector<User *>		users;
		bool					operator==(Channel const &rhs) const;
		bool					operator<(Channel const &rhs) const;
		void					partChannel(User *user);
		std::string				getName();
		std::vector<User *>		getUsers();
		void					setmaxUsers(unsigned int maxUsers);
		unsigned int			getmaxUsers();
		std::map<char, bool>	getModes();
		void					setTopic(std::string topic);
		std::string				getTopic();

		void					sendMessage(std::string message);
		void					switchMode(User *user, std::vector<std::string> messages);
		void					kickUser(Channel *channel, User *user, std::vector<std::string> messages);
		void					execTopic(User *user, std::vector<std::string> messages);
		void					inviteUser(User *user, std::vector<std::string> messages);
		void					setInvited(User *user, Channel &channel_name);
		void					addUser(User *user);
};

void	joinChannel(User *user, Channel *channel);
void	execMessage(std::vector<std::string> messages, User *user, Channel *channel);
bool	findString(std::string str, std::vector<std::string> vec);

#endif