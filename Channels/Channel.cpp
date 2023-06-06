/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:08 by Omar              #+#    #+#             */
/*   Updated: 2023/06/06 23:06:02 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
	this->modes['o'] = false;
	this->modes['k'] = false;
	this->modes['t'] = false;
	this->modes['i'] = false;
	this->modes['l'] = false;
}

Channel::~Channel()
{
}

void Channel::addUser(User user)
{
	std::vector<User>::iterator it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end())
	{
		std::cout << "Error: User " << user.getNickName() << " is already in channel " << this->name << "." << std::endl;
		return;
	}
	this->users.push_back(user);
}

void Channel::removeUser(User user)
{
	std::vector<User>::iterator it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end())
	{
		this->users.erase(it);
	}
	else
		std::cout << "Error: User " << user.getNickName() << " is not in channel " << this->name << "." << std::endl;
}

bool	findString(std::string str, std::vector<std::string> vec)
{
	std::vector<std::string>::iterator it = std::find(vec.begin(), vec.end(), str);
	if (it != vec.end())
		return true;
	return false;
}

std::string Channel::getName()
{
	return this->name;
}

std::vector<User> Channel::getUsers()
{
	return this->users;
}

bool Channel::operator==(Channel const &rhs) const
{
	if (this->name == rhs.name)
		return true;
	return false;
}

void	irc::Server::createChannel(std::string name)
{
	if ((name[0] != '#' && name[0] != '&') || name.length() > 100)
		return;
	if (name.find(' ') != std::string::npos || name.find(',') != std::string::npos)
		return;
	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if (it->getName() == name)
			return;
	}
	Channel newChannel(name);
	this->_channels.push_back(newChannel);
}

void	irc::Server::deleteChannel(const Channel channel)
{
	std::vector<Channel>::iterator it = std::find(this->_channels.begin(), this->_channels.end(), channel);
	if (it != this->_channels.end())
		this->_channels.erase(it);
	else
		return;
}

void	joinChannel(Channel channel, User user)
{
	channel.addUser(user);
}

void	partChannel(Channel channel, User user)
{
	channel.removeUser(user);
}

void	Channel::sendMessage(std::string message)
{
	for (std::vector<User>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		it->addMessage(message);
	}
}

void Channel::switchMode(User user, std::string message)
{
	std::string modes = "oktil";
	(void) user;
	std::string mode_change = message.substr(message.find("MODE") + 5);
	if (mode_change[0] != '+' && mode_change[0] != '-')
		return;
	mode_change = mode_change.erase(mode_change[0]);
	
	std::cout << "the mode change is " << mode_change << std::endl;
}

void Channel::execMessage(std::string message, User user)
{
	if (message.find("JOIN") != std::string::npos)
	{
		joinChannel(*this, user);
		this->sendMessage(":" + user.getNickName() + " JOIN " + this->getName());
	}
	else if (message.find("PART") != std::string::npos)
	{
		partChannel(*this, user);
		this->sendMessage(":" + user.getNickName() + " PART " + this->getName());
	}
	else if (message.find("MODE") != std::string::npos)
	{
		switchMode(user, message);
	}
	else if (message.find("PRIVMSG") != std::string::npos)
	{
		std::string msg = message.substr(message.find("PRIVMSG") + 8);
		this->sendMessage(":" + user.getNickName() + " PRIVMSG " + this->getName() + " :" + msg);
	}
	else if (message.find("NAMES") != std::string::npos)
	{
		std::string names = "";
		for (std::vector<User>::iterator it = this->users.begin(); it != this->users.end(); ++it)
		{
			names += it->getNickName() + " ";
		}
		this->sendMessage(":" + user.getNickName() + " NAMES " + this->getName() + " :" + names);
	}
	else if (message.find("TOPIC") != std::string::npos)
	{
		std::string topic = message.substr(message.find("TOPIC") + 6);
		this->sendMessage(":" + user.getNickName() + " TOPIC " + this->getName() + " :" + topic);
	}
	else if (message.find("KICK") != std::string::npos)
	{
		std::string kick = message.substr(message.find("KICK") + 5);
		this->sendMessage(":" + user.getNickName() + " KICK " + this->getName() + " :" + kick);
	}
}