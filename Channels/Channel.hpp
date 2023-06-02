#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Server/Server.hpp"
#include <algorithm>

class Channel
{
	private:
		std::string name;
		std::vector<std::string> users;
	public:
		Channel(std::string name);
		~Channel();
		void addUser(std::string user);
		bool operator==(Channel const &rhs) const;
		void removeUser(std::string user);
		std::string getName();
		std::vector<std::string> getUsers();
};

bool	findString(std::string str, std::vector<std::string> vec);
void	joinChannel(Channel channel, std::string user);
void	partChannel(Channel channel, std::string user);
#endif