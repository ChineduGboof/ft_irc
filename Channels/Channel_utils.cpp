/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:48:16 by yonamog2          #+#    #+#             */
/*   Updated: 2023/06/15 12:44:18 by Omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void	Channel::kickUser(Channel *channel, User *user, std::vector<std::string> messages)
{
	if (messages.size() < 3 || messages[0] != "KICK" || messages[1] != channel->getName() || user->is_op() == false)
		return;
	std::string nick = messages[2];
	std::vector<User *> users = channel->getUsers();
	for(std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickName() == nick)
		{
			channel->partChannel(*it);
			break;
		}
	}
	// std::string kick = messages[0].substr(messages[0].find("KICK") + 5);
	// channel.sendMessage(":" + user.getNickName() + " KICK " + channel.getName() + " :" + kick);
}

void Channel::switchMode(User *user, std::vector<std::string> messages)
{
	bool mode_bool = false;
	if (messages.size() < 3 || messages[1] != this->getName() || user->is_op() == false)
		return;
	std::string mode = messages[2];
	if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
		return;
	if (messages.size() == 2)
	{
		std::string mode_str = "";
		if (this->modes['o'])
			mode_str += "o";
		if (this->modes['i'])
			mode_str += "i";
		if (this->modes['t'])
			mode_str += "t";
		if (this->modes['k'])
			mode_str += "k";
		if (this->modes['l'] && this->getmaxUsers() != 1000)
			mode_str += "l " + std::atol(this->getmaxUsers());
		
	}
	for (unsigned int i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+')
			mode_bool = true;
		else if (mode[i] == '-')
			mode_bool = false;
		else if (mode[i] == 'o')
		{
			if (mode[0] == '+' && mode.length() > 3)
			{
				std::string nick = mode.substr(3);
				std::vector<User *> users = this->getUsers();
				for(std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
				{
					if ((*it)->getNickName() == nick)
					{
						(*it)->setChannelOp(mode_bool);
						break;
					}
				}
				this->modes['o'] = mode_bool;
			}
			else
				this->modes['o'] = mode_bool;
		}
		else if (mode[i] == 'i')
		{
			this->modes['i'] = mode_bool;
		}
		else if (mode[i] == 't')
		{
			this->modes['t'] = mode_bool;
		}
		else if (mode[i] == 'l' && messages.size() > 3)
		{
			if (mode[0] == '+' && std::atoi(messages[3].c_str()) > 0 && std::atoi(messages[3].c_str()) < 1000)
			{
				this->maxUsers = std::atoi(messages[3].c_str());
				this->modes['l'] = true;
			}
			else
			{
				this->modes['l'] = false;
				this->maxUsers = 1000;
			}
		}
		else
			return;
	}
	std::string mode_str = "";
	for (unsigned int i = 0; i < mode.length(); i++)
	{
		char c = mode[i];
		switch (c)
		{
			case ('+'):{
				mode_str += "+";
				break;
			}
			case ('-'):{
				mode_str += "-";
				break;
			}
			case ('o'):
				mode_str += "o";
				break;
			case ('i'):
				mode_str += "i";
				break;
			case ('t'):
				mode_str += "t";
				break;
			case ('l'):
				mode_str += "l";
				break;
			case ('k'):
				mode_str += "k";
				break;
		}
	}
	std::cout << this->getName() << std::endl;
	std::cout << user->getNickName() << std::endl;
	this->sendMessage(":" + user->getNickName() + " MODE " + this->getName() + " " + mode_str, user->getNickName());
	irc::Server::serverInstance->sendMsg(user->getUserFd(), "MODE " + this->getName() + " " + mode_str + "\r\n");
}

void	Channel::execTopic(User *user, std::vector<std::string> messages)
{
	if(this->modes['t'] == true && user->is_op() == false)
		return;
	if (messages.size() < 3 || this->getName() != messages[1])
		return;
	if (messages.size() == 2)
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "The topic for " + this->getName() + " is " + this->getTopic() + "\r\n");
		return;
	}
	// std::cout << "here" << std::endl;
    unsigned int startIndex = (messages[2] == ":") ? 3 : 2;
   	std::string topic = "";
    if (messages.size() > 3 || messages[2] != ":")
    {
       	topic = messages[startIndex];
       	for (unsigned int i = startIndex + 1; i < messages.size(); i++)
           	topic += " " + messages[i];
    }
    this->setTopic(topic);
}

void	Channel::setInvited(User *user, Channel &channel_name)
{
	user->setInvited(channel_name, true);
}

void	Channel::inviteUser(User *user, std::vector<std::string> messages)
{
	if (this->modes['i'] == true && user->is_op() == false)
		return;
	if (messages.size() < 3 || messages[2] != this->getName())
		return;
	std::string nick = messages[1];
	// std::vector<User *> &users = irc::Server::serverInstance->getUser(); when the server is up
	std::vector<User *> users = this->getUsers();
	for(std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickName() == nick)
		{
			// std::cout << "here cond" << std::endl;
			this->setInvited(*it, *this);
			(*it)->addMessage(":" + user->getNickName() + " INVITE " + (*it)->getNickName() + " " + this->getName());
			break;
		}
	}
}

bool	server_messages(std::vector<std::string> messages)
{
	std::string message = messages[0];
	if (message == "WHOIS" || (message == "MODE" && messages[1] == "FT_irc_server"))
	{
		return (true);
	}
	return (false);
}

void	handle_nickname(User *user, std::vector<std::string> messages)
{
	// if (messages.size() < 2)
	// 	return;
	std::string nick = messages[1];
	std::cout << "nick: " << nick << std::endl;
	if (irc::Server::serverInstance->check_duplicate(nick) == true)
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 433, Nickname is already in use\r\n");
		return;
	}
	irc::Server::serverInstance->sendMsg(user->getUserFd(), "You have changed your nickname to " + nick + "\r\n");
	user->setNickName(nick);
}

void execMessage(std::vector<std::string> messages, User *user, Channel *channel)
{
	std::string message = messages[0];
	std::vector<User *> users = channel->getUsers();
	std::cout << "message: " << message << std::endl;
	if (server_messages(messages) == true)
		return;
	if (message == "JOIN")
	{
		if (channel->getName() == "")
			channel = irc::Server::serverInstance->createChannel(messages[1]);
		joinChannel(user, channel);
		channel->sendMessage(":" + user->getNickName() + " JOIN " + channel->getName());
	}
	else if (message == "PING")
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "PONG\r\n");
		return;
	}
	else if (message == "NICK")
	{
		handle_nickname(user, messages);
	}
	else if (std::find (users.begin(), users.end(), user) == users.end())
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel\r\n");
		return;
	}
	else if (channel->getName() == "")
	{
		if (channel == NULL)
		{
			channel = irc::Server::serverInstance->createChannel(messages[1]);
		}
		// channel->sendMessage(user->getNickName() + " has joined the channel\r\n", NULL);
		joinChannel(user, channel);
		for (size_t i = 0; i < channel->users.size() ; i++)
		{
			// if(channel->users.at(i)->getNickName() == user->getNickName())
			// 	continue;
			std::string msg2 = ":" + user->getNickName() + " JOIN " + messages[1] + " \r\n";
			irc::Server::serverInstance->sendMsg(channel->users.at(i)->getUserFd(), msg2);//":" + user->getNickName() + " JOIN " + messages[1] + "\r\n"
		}
		// irc::Server::serverInstance->sendMsg(user->getUserFd(), ":" + user->getNickName() + " JOIN " + messages[1] + "\r\n");
	}
	else if (message == "MODE")
	{
		channel->switchMode(user, messages);
	}
	else if (message == "PART")
	{
		channel->partChannel(user);
		channel->sendMessage(":" + user->getNickName() + "Has left the channel\r\n");
	}
	else if (message == "TOPIC")
	{
		channel->execTopic(user, messages);
	}
	else if (message == "KICK" && user->is_op() == true)
	{
		channel->kickUser(channel, user, messages);
	}
	else if (message == "INVITE")
	{
		std::string msg = ":" + user->getNickName() + " PART :" + channel->getName() + "\n";
		// std::cout << ":" + user->getNickName() + " PART :" + channel->getName() +"\r\n" << std::endl;
		// std::cout << "fd: " << user->getUserFd() << std::endl;
		// std::cout << "len: " << msg.length() << std::endl;
		irc::Server::serverInstance->sendMsg(user->getUserFd()  , msg);
		channel->sendMessage(":" + user->getNickName() + " PART " + channel->getName() + " :leaving", user->getNickName());
		channel->partChannel(user);
		if(channel->users.size() == 0)
		{
			irc::Server::serverInstance->deleteChannel(channel);
			return ;
		}
	}
	else if (message == "MODE")
	{
		channel->switchMode(user, messages);
	}
	else if (message == "TOPIC")
	{
		channel->execTopic(user, messages);
	}
	else if (message == "KICK" && user->is_op() == true)
	{
		channel->kickUser(channel, user, messages);
	}
	else if (message == "INVITE")
	{
		channel->inviteUser(user, messages);
	}
}