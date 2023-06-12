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
	// std::cout << "here" << std::endl;
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
		std::cout << "Topic: " << this->getTopic() << std::endl;
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

void execMessage(std::vector<std::string> messages, User *user, Channel *channel)
{
	std::string message = messages[0];
	if (message == "JOIN")
	{
		if (channel->getName() == "")
			channel = irc::Server::serverInstance->createChannel(messages[1]);
		joinChannel(user, channel);
		channel->sendMessage(":" + user->getNickName() + " JOIN " + channel->getName());
	}
	else if (channel->getName() == "")
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel");
		return;
	}
	else if (std::find (channel->getUsers().begin(), channel->getUsers().end(), user) == channel->getUsers().end())
	{
		irc::Server::serverInstance->sendMsg(user->getUserFd(), "Error: 442, You're not on that channel");
		return;
	}
	else if (message == "PART")
	{
		channel->partChannel(user);
		channel->sendMessage(":" + user->getNickName() + " PART " + channel->getName());
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