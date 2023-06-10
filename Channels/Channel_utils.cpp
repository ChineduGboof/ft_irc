#include "Channel.hpp"

void	Channel::kickUser(Channel &channel, User user, std::vector<std::string> messages)
{
	if (messages.size() < 3 || messages[0] != "/kick" || messages[1] != channel.getName() || user.is_op() == false)
		return;
	std::string nick = messages[2];
	std::vector<User> users = channel.getUsers();
	for(std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == nick)
		{
			channel.partChannel(*it);
			break;
		}
	}
	// std::string kick = messages[0].substr(messages[0].find("KICK") + 5);
	// channel.sendMessage(":" + user.getNickName() + " KICK " + channel.getName() + " :" + kick);
}

void Channel::switchMode(User user, std::vector<std::string> messages)
{
	bool mode_bool = false;
	if (messages.size() < 3 || messages[1] != this->getName() || user.is_op() == false)
		return;
	std::string mode = messages[2];
	if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
		return;
	std::cout << "here" << std::endl;
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
				std::vector<User> users = this->getUsers();
				for(std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
				{
					if (it->getNickName() == nick)
					{
						it->setChannelOp(mode_bool);
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

void	Channel::execTopic(User user, std::vector<std::string> messages)
{
	if(this->modes['t'] == true && user.is_op() == false)
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

void	Channel::setInvited(User user, Channel &channel_name)
{
	user.setInvited(channel_name, true);
}

void	Channel::inviteUser(User &user, std::vector<std::string> messages)
{
	if (this->modes['i'] == true && user.is_op() == false)
		return;
	if (messages.size() < 3 || messages[2] != this->getName())
		return;
	std::string nick = messages[1];
	// std::vector<User *> &users = irc::Server::serverInstance->getUser(); when the server is up
	std::cout << user.getNickName() << std::endl;
	std::vector<User> users = this->getUsers();
	std::cout << this->getUsers().front().getNickName() << std::endl;
	for(std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		std::cout << "here" <<  it->getNickName() << std::endl;
		if (it->getNickName() == nick)
		{
			this->setInvited(*it, *this);
			it->addMessage(":" + user.getNickName() + " INVITE " + it->getNickName() + " " + this->getName());
			break;
		}
	}
}

void Channel::execMessage(std::vector<std::string> messages, User &user)
{
	if (std::find (this->users.begin(), this->users.end(), user) == this->users.end())
		return;
	std::string message = messages[0];
	if (message.find("JOIN") != std::string::npos)
	{
		this->joinChannel(user);
		this->sendMessage(":" + user.getNickName() + " JOIN " + this->getName());
	}
	else if (message.find("PART") != std::string::npos)
	{
		this->partChannel(user);
		this->sendMessage(":" + user.getNickName() + " PART " + this->getName());
	}
	else if (message.find("MODE") != std::string::npos)
	{
		switchMode(user, messages);
	}
	else if (message.find("PRIVMSG") != std::string::npos)
	{
		std::string msg = message.substr(message.find("PRIVMSG") + 8);
		this->sendMessage(":" + user.getNickName() + " PRIVMSG " + this->getName() + " :" + msg);
	}
	else if (message.find("TOPIC") != std::string::npos)
	{
		execTopic(user, messages);
	}
	else if (message.find("KICK") != std::string::npos && user.is_op() == true)
	{
		kickUser(*this, user, messages);
	}
	else if (message.find("INVITE") != std::string::npos)
	{
		inviteUser(user, messages);
	}
}