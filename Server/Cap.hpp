/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 01:51:47 by gboof             #+#    #+#             */
/*   Updated: 2023/06/07 04:25:38 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAP_HPP
#define CAP_HPP
# include <iostream>
# include <algorithm>
# include <string>
# include <vector>
# include <deque>
# include <map>
# include <stdexcept>
#include <iomanip>
#include <ios>
# include <unistd.h>
#include "Server.hpp"
#include "user.hpp"
#include "Responses.hpp"

using namespace irc;

class Cap : public Server, public User {
public:
    Cap();
    Cap(User* user) : _user(user) {}
    ~Cap();

    void Handshake();
    // void answer(Responses::ResponseCode responseCode, const std::string& trail);
    // void answer(const std::string& command, const std::string& target);
    void answer(const std::string& command, const std::string& target, const std::string& trailing);

private:
    User* _user;
};


#endif // !CAP_HPP