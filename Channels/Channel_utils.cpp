#include "Channel.hpp"

void	Channel::kickUser(Channel channel, User user, std::vector<std::string> messages)
{
	if (messages.size() < 3 || messages[0] != "/kick" || messages[1] != channel.getName() || user.is_op() == false)
		return;
	std::string nick = messages[2];
	std::vector<User> users = channel.getUsers();
	for(std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == nick)
		{
			channel.removeUser(*it);
			break;
		}
	}
	std::string kick = messages[0].substr(messages[0].find("KICK") + 5);
	channel.sendMessage(":" + user.getNickName() + " KICK " + channel.getName() + " :" + kick);
}

void Channel::switchMode(User user, std::vector<std::string> messages)
{
	if (messages.size() < 3 || messages[0] != "/mode" || messages[1] != this->getName() || user.is_op() == false)
		return;
	std::string mode = messages[2];
	if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-'))
		return;
	for (unsigned int i = 1; i < mode.length(); i++)
	{
		if (mode[i] == 'o')
		{
			if (std::find(this->users.begin(), this->users.end(), user) == this->users.end())
				return;
			std::cout << "switchMode & mode = " << mode << std::endl;
			if (mode[0] == '+')
				this->modes['o'] = true;
			else
				this->modes['o'] = false;
		}
		else if (mode[i] == 'k')
		{
			if (std::find(this->users.begin(), this->users.end(), user) == this->users.end())
				return;
			if (mode[0] == '+')
				this->modes['k'] = true;
			else
				this->modes['k'] = false;
		}
		else if (mode[i] == 't')
		{
			if (std::find(this->users.begin(), this->users.end(), user) == this->users.end())
				return;
			if (mode[0] == '+')
				this->modes['t'] = true;
			else
				this->modes['t'] = false;
		}
		else if (mode[i] == 'i')
		{
			if (std::find(this->users.begin(), this->users.end(), user) == this->users.end())
				return;
			if (mode[0] == '+')
				this->modes['i'] = true;
			else
				this->modes['i'] = false;
		}
		else if (mode[i] == 'l')
		{
			if (std::find(this->users.begin(), this->users.end(), user) == this->users.end())
				return;
			if (mode[0] == '+')
				this->modes['l'] = true;
			else
				this->modes['l'] = false;
		}
		else
			return;
	}
}

void Channel::execMessage(std::vector<std::string> messages, User user)
{
	if (std::find (this->users.begin(), this->users.end(), user) == this->users.end())
		return;
	std::string message = messages[0];
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
		switchMode(user, messages);
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
	else if (message.find("KICK") != std::string::npos && user.is_op() == true)
	{
		kickUser(*this, user, messages);
	}
}