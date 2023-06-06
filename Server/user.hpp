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
#include <iostream>
# include <cstdio>
# include <deque>
# include <list>
# include <sys/socket.h>
# include <netinet/in.h>
#include "Utils.hpp"

class User
{
private:
    int user_fd;
    std::string real_name;
    std::string nick_name;
    std::string user_name;

    std::string _dataBuffer;
    std::deque<std::vector<std::string> > _incomingMsgs;
    std::deque<std::string> _outgoingMsgs;
    // bool        is_auth;
    // bool        joined_server;
public:
    User( void );
    User(int fd);
    std::string getRealName( void );
    std::string getNickName( void );
    std::string getUserName( void );
    int  getUserFd( void );

    void setNickName( std::string nick );
    void setUserName( std::string user );
    void setRealName( std::string real );

    size_t receive();
    std::deque<std::vector<std::string> >& getMessages();
    std::deque<std::string>& getOutgoingMsg();
    void setOutgoingMsg( std::string msg );

    ~User( void );
};



#endif