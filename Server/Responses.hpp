/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gboof <gboof@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:59:57 by gboof             #+#    #+#             */
/*   Updated: 2023/06/07 00:05:24 by gboof            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSES_HPP
# define RESPONSES_HPP

# include <string>
# include <iostream>

namespace irc {
    class Responses {
    public:
        enum ResponseCode {
            // Positive replies
            RPL_WELCOME = 001,
            RPL_YOURHOST = 002,
            RPL_CREATED = 003,
            RPL_MYINFO = 004,
            RPL_ISUPPORT = 005,
            RPL_UMODEIS = 221,
            RPL_LUSERCLIENT = 251,
            RPL_LUSEROP = 252,
            RPL_LUSERUNKNOWN = 253,
            RPL_LUSERCHANNELS = 254,
            RPL_LUSERME = 255,
            RPL_LOCALUSERS = 265,
            RPL_GLOBALUSERS = 266,
            RPL_NONE = 300,
            RPL_ENDOFWHO = 315,
            RPL_ENDOFWHOIS = 318,
            RPL_CHANNELMODEIS = 324,
            RPL_NOTOPIC = 331,
            RPL_TOPIC = 332,
            RPL_TOPICWHOTIME = 333,
            RPL_INVITING = 341,
            RPL_WHOREPLY = 352,
            RPL_NAMREPLY = 353,
            RPL_ENDOFNAMES = 366,
            RPL_ENDOFBANLIST = 368,
            RPL_MOTDSTART = 375,
            RPL_MOTD = 372,
            RPL_ENDOFMOTD = 376,
            RPL_YOUREOPER = 381,
            RPL_REHASHING = 382,
            RPL_HELPTXT = 705,
            RPL_ENDOFHELP = 706,
            
            // Error replies
            ERR_NOSUCHNICK = 401,
            ERR_NOSUCHSERVER = 402,
            ERR_NOSUCHCHANNEL = 403,
            ERR_CANNOTSENDTOCHAN = 404,
            ERR_TOOMANYCHANNELS = 405,
            ERR_INPUTTOOLONG = 417,
            ERR_UNKNOWNCOMMAND = 421,
            ERR_NOMOTD = 422,
            ERR_NONICKNAMEGIVEN = 431,
            ERR_ERRONEUSNICKNAME = 432,
            ERR_NICKNAMEINUSE = 433,
            ERR_NICKCOLLISION = 436,
            ERR_USERNOTINCHANNEL = 441,
            ERR_NOTONCHANNEL = 442,
            ERR_USERONCHANNEL = 443,
            ERR_NEEDMOREPARAMS = 461,
            ERR_ALREADYREGISTERED = 462,
            ERR_PASSWDMISMATCH = 464,
            ERR_CHANNELISFULL = 471,
            ERR_INVITEONLYCHAN = 473,
            ERR_BADCHANNELKEY = 475,
            ERR_BADCHANMASK = 476,
            ERR_NOPRIVILEGES = 481,
            ERR_CHANOPRIVSNEEDED = 482,
            ERR_NOOPERHOST = 491,
            ERR_UMODEUNKNOWNFLAG = 501,
            ERR_USERSDONTMATCH = 502,
            ERR_HELPNOTFOUND = 524,
            ERR_INVALIDKEY = 525,
            ERR_INVALIDMODEPARAM = 696
        };
    };
}
#endif
