/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:08 by Omar              #+#    #+#             */
/*   Updated: 2023/06/04 22:03:14 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
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

void Channel::execMessage(std::string message, User user)
{
	
}