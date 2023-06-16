/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Omar <Oabushar@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:48:16 by yonamog2          #+#    #+#             */
/*   Updated: 2023/06/16 14:09:32 by Omar             ###   ########.fr       */
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
			channel->sendMessage(user->getNickName() + " has kicked " + nick + " from " + channel->getName() + "!", "");
			irc::Server::serverInstance->sendMsg((*it)->getUserFd(),":" + (*it)->getNickName() + " PART :" + channel->getName() + "\n");
			channel->partChannel(*it);
			break;
		}
	}

}

void Channel::switchMode(User* user, std::vector<std::string> messages)
{
    bool mode_bool = false;
    if (messages.size() < 2 || messages[1] != this->getName() || !user->is_op())
        return;

    std::string mode = messages[2];
    if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
        return;

    std::string modeStr = "";
    std::map<char, bool> modeFlags;
    modeFlags['o'] = this->modes['o'];
    modeFlags['i'] = this->modes['i'];
    modeFlags['t'] = this->modes['t'];
    modeFlags['l'] = this->modes['l'];
    modeFlags['k'] = this->modes['k'];

    for (size_t i = 0; i < mode.length(); i++)
    {
        char c = mode[i];

        switch (c)
        {
            case '+':
                mode_bool = true;
                modeStr += "+";
                break;

            case '-':
                mode_bool = false;
                modeStr += "-";
                break;

            case 'o':
            case 'i':
            case 't':
                modeStr += c;
                modeFlags[c] = mode_bool;
                break;

            case 'l':
                if (messages.size() > 3 && mode[0] == '+' && std::atoi(messages[3].c_str()) > 0 && std::atoi(messages[3].c_str()) < 1000)
                {
                    modeStr += c;
                    modeFlags[c] = true;
                    this->maxUsers = std::atoi(messages[3].c_str());
                }
                else
                {
                    modeFlags[c] = false;
                    this->maxUsers = 1000;
                }
                break;

            case 'k':
                if (messages.size() > 3 && mode[0] == '+')
                {
                    modeStr += c;
                    modeFlags[c] = true;
                    this->key = messages[3];
                }
                else
                {
                    modeFlags[c] = false;
                    this->key = "";
                }
                break;

			}
			case ('k'):
				mode_str += "k";
				break;
		}
	}
	this->sendMessage(":" + user->getNickName() + " MODE " + this->getName() + " " + mode_str + "\r\n", "");
}


void	Channel::execTopic(User *user, std::vector<std::string> messages)
{
	if(this->modes['t'] == true && user->is_op() == false)
		return;
	if (messages.size() < 3 || this->getName() != messages[1])
		return;
	if (messages.size() == 2)
	{
		this->sendMessage(":irc 331 " + user->getNickName() + " " + this->getName() + " " + this->getTopic(), "");
		return;
	}
	if (modes['t'] == true && user->is_op() == false)
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "482 " + user->getNickName() + " " + this->getName() + " :You're not channel operator\r\n");
		return;
	}
    unsigned int startIndex = (messages[2] == ":") ? 3 : 2;
   	std::string topic = "";
    if (messages.size() > 3 || messages[2] != ":")
    {
       	topic = messages[startIndex];
       	for (unsigned int i = startIndex + 1; i < messages.size(); i++)
           	topic += " " + messages[i];
    }
    this->setTopic(topic);
	this->sendMessage(":irc 332 " + user->getNickName() + " " + this->getName() + " " + this->getTopic(), "");
}

void	Channel::inviteUser(User *user, std::vector<std::string> messages)
{
	if (this->modes['i'] == true && user->is_op() == false)
		return;
	if (messages.size() < 3 || messages[2] != this->getName())
		return;
	std::string nick = messages[1];
	for(std::vector<User *>::iterator it = irc::Server::serverInstance->getUser().begin() ; it != irc::Server::serverInstance->getUser().end() ; ++it)
	{
		if ((*it)->getNickName() == nick)
		{
			(*it)->setInvited(*this, true);
			irc::Server::serverInstance->sendMsg((*it)->getUserFd(), ":" + user->getNickName() + " INVITE " + nick + " " + this->getName() + "\r\n");
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

void join_channel(std::string chnl_name , User *user, Channel *channel, std::string password)
{
	if (channel == NULL)
	{
		channel = irc::Server::serverInstance->createChannel(chnl_name, password);
	}
	if(password != channel->getKey())
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), ":irc 475 " + user->getNickName() + " " + chnl_name + " :Incorrect Channel Key\n");
		return;
	}
	if (channel->users.size() >= channel->getmaxUsers())
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), ":irc 471 " + user->getNickName() + " " + chnl_name + " :Cannot join channel (+l)\n");
		return;
	}
	if (channel->getModes()['i'] == true && user->getInvited(*channel) == false)
	{
		std::cout << "THISSSSSSSS CONDDDD" << std::endl;
		irc::Server::serverInstance->sendMsg(user->getUserFd(), ":irc 473 " + user->getNickName() + " " + chnl_name + " :Cannot join channel (+i)\n");
		return;
	}
	joinChannel(user, channel);
	irc::Server::serverInstance->sendMsg(user->getUserFd(), ":irc 332 " + user->getNickName() + " " + channel->getName() + " " + channel->getTopic() + "\r\n");
	for (size_t i = 0; i < channel->users.size() ; i++)
	{
		std::string msg2 = ":" + user->getNickName() + " JOIN " + chnl_name + " \r\n";
		irc::Server::serverInstance->sendMsg(channel->users.at(i)->getUserFd(), msg2);
	}
}

void execMessage(std::vector<std::string> messages, User *user)
{
	std::string message = messages[0];
	Channel *channel = irc::Server::serverInstance->getChannel(messages[1]);
	if (message == "INVITE")
		channel = irc::Server::serverInstance->getChannel(messages[2]);
	if (server_messages(messages) == true)
		return;
	if (message == "JOIN")
	{
		size_t x = 0;
		while (x < user->_channelToJoin.size())
		{
			channel = irc::Server::serverInstance->getChannel(user->_channelToJoin.at(x));
			if(user->_channelKeys.size())
				join_channel(user->_channelToJoin.at(x), user, channel, user->_channelKeys.at(x));
			else
				join_channel(user->_channelToJoin.at(x), user, channel, "");
			x++;
		}
		
	}
	else if (message == "PING")
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "PONG\r\n");
		return;
	}
	else if (message == "PRIVMSG")
	{
		if (messages.size() < 3)
			return;
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
				return;
			}
			channel = *it;
			msg = messages[2];
			std::vector<User *>::iterator it2;
			for (it2 = channel->users.begin(); it2 != channel->users.end(); ++it2)
			{
				if((*it2)->getNickName() == user->getNickName())
				{
					std::cout << "found user" << std::endl;
					break ;
				}
			}
			if(it2 == channel->users.end())
			{
				irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 401, User not in channel\r\n");
				return;
			}
			for (unsigned int i = 3; i < messages.size(); i++)
				msg += " " + messages[i];
			std::string msg2 = ":" + user->getNickName() + " PRIVMSG " + messages[1] + " " +  msg + "\r\n";
			channel->sendMessage(msg2, user->getNickName());
		}
		return;
	}
	else if (message == "NICK")
	{
		handle_nickname(user, messages);
	}
	if (channel == NULL)
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel\r\n");
		return;
	}
	if (message == "PART")
	{
		std::string msg = ":" + user->getNickName() + " PART :" + channel->getName() + "\r\n";
		irc::Server::serverInstance->sendMsg(user->getUserFd()  , msg);
		channel->sendMessage(":" + user->getNickName() + " PART " + channel->getName() + " :leaving", user->getNickName());
		if(channel->partChannel(user) == true)
		{
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