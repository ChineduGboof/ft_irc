#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Server/Server.hpp"
#include "temp_User.hpp"
#include <algorithm>
#include <map>

class Channel
{
	private:
		std::string name;
		std::vector<User> users;
		char mode;
	public:
		Channel(std::string name);
		~Channel();
		void				addUser(User user);
		bool				operator==(Channel const &rhs) const;
		void				removeUser(User user);
		std::string			getName();
		std::vector<User>	getUsers();
		void				sendMessage(std::string message);
		void				setMode(char mode);
};

bool	findString(std::string str, std::vector<std::string> vec);
void	joinChannel(Channel channel, User user);
void	partChannel(Channel channel, User user);
#endif