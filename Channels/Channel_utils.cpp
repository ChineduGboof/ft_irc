/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:48:16 by yonamog2          #+#    #+#             */
/*   Updated: 2023/06/14 19:05:00 by Omar             ###   ########.fr       */
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
			irc::Server::serverInstance->sendMsg((*it)->getUserFd(), "You have been invited to channel " + this->getName() + "\r\n");
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
	// std::cout << "message: " << message << std::endl;
	if (server_messages(messages) == true)
		return;
	if (message == "JOIN")
	{
		if (channel->getName() == "")
			channel = irc::Server::serverInstance->createChannel(messages[1]);
		joinChannel(user, channel);
		// std::cout << "got : " << channel->users.size() << std::endl;
		for (size_t i = 0; i < channel->users.size() ; i++)
		{
			std::string msg2 = ":" + user->getNickName() + "!" + user->getUserName() + "@" + irc::Server::serverInstance->getRemoteIP() +  " JOIN " + messages[1] + " \r\n";
			irc::Server::serverInstance->sendMsg(channel->users.at(i)->getUserFd(), msg2);//":" + user->getNickName() + " JOIN " + messages[1] + "\r\n"
		}
		// irc::Server::serverInstance->sendMsg(user->getUserFd(), ":" + user->getNickName() + " JOIN " + messages[1] + "\r\n");
	}
	else if (message == "PING")
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "PONG\r\n");
		return;
	}
	else if (message == "PRIVMSG")
	{
		// int i = 0;
		// for(std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it)
		// {
		// 	std::cout << i++ <<":" << *it << std::endl;

		// }
		std::string msg = messages[2];
		if (messages[1][0] != '#')
		{
			if(irc::Server::serverInstance->getFdByNick(messages[1]) != -1)
            {
				msg = messages[2];
				for (unsigned int i = 3; i < messages.size(); i++)
					msg += " " + messages[i];
				std::string msg2 = ":" + user->getNickName() + " PRIVMSG " + messages[1] + " " +  msg + "\r\n";
				std::cout << "sending: " << msg2 << std::endl;
				irc::Server::serverInstance->sendMsg(irc::Server::serverInstance->getFdByNick(messages[1]), msg2);
        	}
			else
			{
				irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 401, No such nick/channel\r\n");
			}
		}
		else if (messages[1][0] == '#')
		{
			std::vector<Channel *> channels = irc::Server::serverInstance->getChannels();
			std::vector<Channel *>::iterator it;
			std::cout << "channel name: " << messages[1] << std::endl;
			for (it = channels.begin();  it != channels.end(); ++it)
			{
				if ((*it)->getName() == messages[1]){

				std::cout << "channel nammme: " << (*it)->getName() << std::endl;
					break;
				}
			}
			if (it == channels.end())
			{
				irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 401, No such nick/channel\r\n");
				std::cout << "here lol" << std::endl;
				return;
			}
			channel = *it;
			msg = messages[2];
			for (unsigned int i = 3; i < messages.size(); i++)
				msg += " " + messages[i];
			// std::string msg2 = ":" + user->getNickName() + "!" + user->getUserName() + "@localhostPRIVMSG " + channel->getName() + " :" + msg + "\r\n";
			std::string msg2 = ":" + user->getNickName() + " PRIVMSG " + messages[1] + " " +  msg + "\r\n";
			channel->sendMessage(msg2, user->getNickName());
		}
	}
	else if (message == "NICK")
	{
		handle_nickname(user, messages);
	}
	// else if (std::find (users.begin(), users.end(), user) == users.end())
	// {
	// 	std::cout << "here 1" << message << std::endl;
	// 	irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel\r\n");
	// 	return;
	// }
	else if (channel->getName() == "")
	{
		channel = irc::Server::serverInstance->getChannel(messages[1]);
		if (!channel)
			return;
		std::cout << "here 2: " << channel->getName() << std::endl;
		// irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel\r\n");
		// return;
	}
	if (message == "PART")
	{
		// std::cout << ":" + user->getNickName() + " PART " + channel->getName() + " :leaving" +"\r\n" << std::endl;
		channel->partChannel(user);
		channel->sendMessage(user->getNickName() + "PART :" + channel->getName() + "\r\n", user->getNickName());
		irc::Server::serverInstance->sendMsg(user->getUserFd()  , ":" + user->getNickName() + "!" + user->getUserName() + "@" + irc::Server::serverInstance->getRemoteIP() + " PART " + channel->getName() + " :leaving" +"\r\n");
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