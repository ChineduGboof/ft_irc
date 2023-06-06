#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <sstream>
# include <limits>
# include <exception>
# include <cstdlib>
# include <queue>
# include <ctime>
# include <algorithm>
# include <iostream>
# include <deque>
class User
{
private:
    int user_fd;
    std::string real_name;
    std::string nick_name;
    std::string user_name;
	std::deque<std::string> messages;

public:
    User( void );
    User(int fd);
    std::string getRealName( void );
    std::string getNickName( void );
    std::string getUserName( void );
    int  getUserFd( void );

	bool operator==(User const &rhs) const;
    void setNickName( std::string nick );
    void setUserName( std::string user );
    void setRealName( std::string real );
	void addMessage(std::string message);
    ~User( void );
};



#endif