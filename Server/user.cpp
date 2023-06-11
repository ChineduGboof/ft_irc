#include "user.hpp"

User::User( void ){}

User::~User( void ) {
    _incomingMsgs.clear();
    _outgoingMsgs.clear();
}

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

bool User::getIsAuth( void )
{
    return (this->is_auth);
}
void User::setIsAuth( bool auth )
{
    this->is_auth = auth;
}

User::User(int fd)
{  
    this->user_fd = fd;
    this->nick_name = "";
    this->user_name = "";
    this->real_name = "";
	this->is_channel_op = false;
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

std::deque<std::string> User::getMessagesDeque()
{
	return this->messages;
}

bool User::operator==(User const &rhs) const
{
	if (this->user_fd == rhs.user_fd)
		return true;
	return false;
}

void	User::setChannelOp(bool op)
{
	this->is_channel_op = op;
}

bool	User::is_op()
{
	return this->is_channel_op;
}

void	User::setInvited(Channel channel_name, bool invited)
{
	this->invited_channels[channel_name] = invited;
}

bool	User::getInvited(Channel channel_name)
{
	return this->invited_channels[channel_name];
}

// bool	User::getInvited(Channel channel_name)
// {
// 	return this->invited_channels[channel_name];
// }
/**
 * @brief Receive data from the user and process it.
 *
 * This function reads data from the user's socket and processes it. It appends the received data
 * to the internal data buffer, splits the buffer into messages using the delimiter "\r\n",
 * and stores each message in the _incomingMsgs vector after splitting it into words using the
 * delimiter " ".
 *
 * Note: The commented-out code block is used for printing the messages stored in the _incomingMsgs vector.
 *
 * @return The number of bytes received from the user, or a value <= 0 if an error occurred or the connection is closed.
 */
size_t User::receive() {

    char buffer[1024];
    size_t bytesRead = recv(user_fd, buffer, sizeof(buffer), 0);
    buffer[bytesRead] = '\0';
    if (bytesRead <= 0)
        return bytesRead;

    _dataBuffer = "";
    _dataBuffer = buffer;
    // _dataBuffer.append(buffer);
    // std::cout << "----------------------------------------------\n";
    // std::cout << "msg[" <<user_fd<<"]: "  << _dataBuffer << std::endl;
    // std::cout << "----------------------------------------------\n";
    // std::queue<std::string> temp = utils::splitByDelimiter(_dataBuffer, "\r\n");
    // while (!temp.empty()) {
    //     std::vector<std::string> splitWords = utils::splitBySpace(temp.front());
    //     _incomingMsgs.push_back(splitWords);
    //     temp.pop();
    // }

    // Print the messages stored in the _incomingMsgs vector
    // int i = 0;
    // for (std::deque<std::vector<std::string> >::iterator it = _incomingMsgs.begin(); it != _incomingMsgs.end(); ++it) {
    //     for (std::vector<std::string>::iterator strIt = it->begin(); strIt != it->end(); ++strIt) {
    //         std::cout << "Index " << i++ << " => " << *strIt << std::endl;  // Print each word in a new line
    //     }
    // }
    return bytesRead;
}


std::deque<std::vector<std::string> >& User::getMessages() {
    return _incomingMsgs;
}

std::deque<std::string>& User::getOutgoingMsg() {
    return _outgoingMsgs;
}

void User::setOutgoingMsg( std::string msg ) {
    _outgoingMsgs.push_back( msg );
}

void User::printOutgoingMsgs() {
    for (std::deque<std::string>::const_iterator it = _outgoingMsgs.begin(); it != _outgoingMsgs.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

