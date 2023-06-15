/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:04:08 by Omar              #+#    #+#             */
/*   Updated: 2023/06/15 19:25:33 by Omar             ###   ########.fr       */
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
	this->topic = "";
	this->key = "";
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

std::string	Channel::getKey()
{
	return this->key;
}
void	Channel::setKey(std::string password)
{
	this->key = password;
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

bool Channel::partChannel(User *user)
{
	std::vector<User *>::iterator it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end() && this->users.size() == 1)
	{
		user->setChannelOp(false);
		irc::Server::serverInstance->sendMsg(user->getUserFd()  , ":" + user->getNickName() + " PART " + this->getName() + " :leaving " +"\r\n");
		this->users.erase(it);
		irc::Server::serverInstance->deleteChannel(this);
		return false;
	}
	else if (it != this->users.end())
	{
		user->setChannelOp(false);
		this->users.erase(it);
		return true;
	}
	else
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You are not in channel " + this->getName() + ".\r\n");
		return false;
	}
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
Channel	*irc::Server::createChannel(std::string name, std::string password)
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
	newChannel->setKey(password);
	// std::cout << "inisde_pass: " << newChannel->getKey() << std::endl;
	this->_channels.push_back(newChannel);
	return newChannel;
}

Channel *irc::Server::getChannel(std::string name)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if ((*it)->getName() == name)
			return (*it);
	}
	return NULL;
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

void	Channel::sendMessage(std::string message, std::string sender)
{
	for (std::vector<User *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		if((*it)->getNickName() != sender)
			irc::Server::serverInstance->sendMsg((*it)->getUserFd(), message + "\r\n");
	}
}
