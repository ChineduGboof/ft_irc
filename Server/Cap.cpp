/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 01:55:45 by gboof             #+#    #+#             */
/*   Updated: 2023/06/07 04:30:33 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cap.hpp"

void Cap::Handshake() {
    std::vector<User*> clients;
    std::deque<std::vector<std::string> > data;
    
    try {
        clients = getUser();
        data = getMessages();
    } catch (Cap& e) {
        return;
    }

    if (!data.empty() && !data[0].empty() && data[0][0] == "LS") {
        answer(utils::convertToString("CAP"), _user->getNickName(), "LS");
    } else if (!clients.empty() && data[0][0] == "END") {
        std::string nickName = clients[0]->getNickName();
        if (nickName.empty()) {
            return;
        }

        answer(utils::convertToString(Responses::RPL_WELCOME), "Welcome to the " + utils::convertToString(SERVER) + " Network, " + nickName, "");
        answer(utils::convertToString(Responses::RPL_YOURHOST), "Your host is Chinedu, Yonathan & Omar", "");
        answer(utils::convertToString(Responses::RPL_CREATED), "This server was created to meet your chat needs", "");
        answer(utils::convertToString(Responses::RPL_MYINFO), utils::convertToString(SERVER), "");
        answer(utils::convertToString(Responses::RPL_ISUPPORT), "Kindly Support this Project", "");
        answer(utils::convertToString(Responses::RPL_LUSERCLIENT), "There are a number of users on the Server", "");
        answer(utils::convertToString(Responses::RPL_LUSERME), "Have fun", "");
    }
}

void Cap::answer(const std::string& command, const std::string& target, const std::string& trailing) {
    if (!_user) {
        return;
    }

    std::string response = ":" + utils::convertToString(SERVER) + " " + command + " " + target;
    if (!trailing.empty()) {
        response += " :" + trailing;
    }
    response += "\r\n";

    _user->setOutgoingMsg(response);
}
