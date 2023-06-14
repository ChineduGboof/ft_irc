/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:08 by Omar              #+#    #+#             */
/*   Updated: 2023/06/14 14:26:52 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
	this->maxUsers = 1000;
	this->modes['o'] = false;
	this->modes['k'] = false;
	this->modes['t'] = false;
	this->modes['i'] = false;
	this->modes['l'] = false;
}

std::map<char, bool> Channel::getModes()
{
	return this->modes;
}

void	Channel::setmaxUsers(unsigned int maxUsers)
{
	this->maxUsers = maxUsers;
}

unsigned int Channel::getmaxUsers()
{
	return this->maxUsers;
}

void	Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

std::string Channel::getTopic()
{
	return this->topic;
}

void	Channel::addUser(User *user)
{
	this->users.push_back(user);
}

Channel::~Channel()
{
	// irc::Server::serverInstance->deleteChannel(this);
}

void joinChannel(User *user, Channel *channel)
{
	// std::vector<User *>::iterator it = std::find(channel->getUsers().begin(), channel->getUsers().end(), user);
	// if (it != channel->getUsers().end())
	// {
	// 	irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 443, You are already in channel " + channel->getName() + ".\r\n");
	// 	return;
	// }
	if (channel->getUsers().size() >= channel->getmaxUsers())
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 471, Channel " + channel->getName() + " is full.\r\n");
		return;
	}
	if (channel->getModes()['i'] == true && user->getInvited(*channel) == false)
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 473, You are not invited to channel " + channel->getName() + ".\r\n");
		return;
	}
	if (channel->getUsers().size() == 0)
	{
		user->setChannelOp(true);
	}
	channel->addUser(user);
}

void Channel::partChannel(User *user)
{
	std::vector<User *>::iterator it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end())
	{
		user->setChannelOp(false);
		this->users.erase(it);
	}
	else
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You are not in channel " + this->getName() + ".\r\n");
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

std::vector<User *> Channel::getUsers()
{
	return this->users;
}

bool Channel::operator==(Channel const &rhs) const
{
	if (this->name == rhs.name)
		return true;
	return false;
}

bool Channel::operator<(Channel const &rhs) const
{
	if (this->name < rhs.name)
		return true;
	return false;
}
Channel	*irc::Server::createChannel(std::string name)
{
	if ((name[0] != '#' && name[0] != '&') || name.length() > 100)
		return (NULL);
	if (name.find(' ') != std::string::npos || name.find(',') != std::string::npos)
		return (NULL);
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if ((*it)->getName() == name)
			return (*it);
	}
	Channel *newChannel = new Channel(name);
	this->_channels.push_back(newChannel);
	return newChannel;
}

void irc::Server::deleteChannel(Channel* channel)
{
    std::vector<Channel*>::iterator it = std::find(this->_channels.begin(), this->_channels.end(), channel);
    if (it != this->_channels.end())
    {
        delete *it;
        this->_channels.erase(it);
    }
    else
        return;
}

void	Channel::sendMessage(std::string message)
{
	for (std::vector<User *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		irc::Server::serverInstance->sendMsg((*it)->getUserFd(), message + "\r\n");
	}
}
