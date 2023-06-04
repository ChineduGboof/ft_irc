#ifndef TEMP_USER_HPP
#define TEMP_USER_HPP

#include "Channel.hpp"
#include <deque>

class User
{
	private:
		std::string name;
		std::string nick;
		std::deque<std::string> messages;
	public:
		User(std::string name, std::string nick){
			this->name = name;
			this->nick = nick;
		};
		~User(){};
		std::string getName(){return this->name;};
		std::string getNick(){return this->nick;};
		void addMessage(std::string message){this->messages.push_back(message);};
		bool operator==(User const &rhs) const
		{
			if (this->nick == rhs.nick)
				return true;
			return false;
		};
};

#endif