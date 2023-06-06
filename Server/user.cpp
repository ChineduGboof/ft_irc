#include "user.hpp"

User::User( void ){}

User::~User( void ) {}

std::string User::getRealName( void )
{
    return (this->user_name);
}
std::string User::getNickName( void )
{
    return (this->nick_name);
}
std::string User::getUserName( void )
{
    return (this->user_name);
}
int User::getUserFd( void )
{
    return (this->user_fd);
}

User::User(int fd)
{  
    this->user_fd = fd;
    this->nick_name = "";
    this->user_name = "";
    this->real_name = "";
}

void User::setRealName( std::string name )
{
    this->real_name = name;
}

void User::setNickName( std::string nick )
{
    this->nick_name = nick;
}

void User::setUserName( std::string user )
{
    this->user_name = user;
}

// Channel changes:

void User::addMessage(std::string message)
{
	this->messages.push_back(message);
}

bool User::operator==(User const &rhs) const
{
	if (this->user_fd == rhs.user_fd)
		return true;
	return false;
}