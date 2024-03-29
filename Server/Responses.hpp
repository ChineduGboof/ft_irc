/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cegbulef <cegbulef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:59:57 by gboof             #+#    #+#             */
/*   Updated: 2023/06/13 11:40:05 by cegbulef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSES_HPP
# define RESPONSES_HPP

# define SERVER "Ft_IRC"
# define HOST_P "002"

// Positive replies
# define ONLINE  1
# define RPL_YOURHOST "002"
# define RPL_WELCOME "001"
# define RPL_CREATED "003"
# define RPL_MYINFO "004"
# define RPL_ISUPPORT "005"
# define RPL_UMODEIS "221"
# define RPL_LUSERCLIENT "251"
# define RPL_LUSEROP "252"
# define RPL_LUSERUNKNOWN "253"
# define RPL_LUSERCHANNELS "254"
# define RPL_LUSERME "255"
# define RPL_LOCALUSERS "265"
# define RPL_GLOBALUSERS "266"
# define RPL_NONE "300"
# define RPL_ENDOFWHO "315"
# define RPL_ENDOFWHOIS "318"
# define RPL_CHANNELMODEIS "324"
# define RPL_NOTOPIC "331"
# define RPL_TOPIC "332"
# define RPL_TOPICWHOTIME "333"
# define RPL_INVITING "341"
# define RPL_WHOREPLY "352"
# define RPL_NAMREPLY "353"
# define RPL_ENDOFNAMES "366"
# define RPL_ENDOFBANLIST "368"
# define RPL_MOTDSTART "375"
# define RPL_MOTD "372"
# define RPL_ENDOFMOTD "376"
# define RPL_YOUREOPER "381"
# define RPL_REHASHING "382"
# define RPL_HELPTXT "705"
# define RPL_ENDOFHELP "706"

// Error replies
# define ERR_NOSUCHNICK "401"
# define ERR_NOSUCHSERVER "402"
# define ERR_NOSUCHCHANNEL "403"
# define ERR_CANNOTSENDTOCHAN "404"
# define ERR_TOOMANYCHANNELS "405"
# define ERR_INPUTTOOLONG "417"
# define ERR_UNKNOWNCOMMAND "421"
# define ERR_NOMOTD "422"
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKNAMEINUSE "433"
# define ERR_NICKCOLLISION "436"
# define ERR_USERNOTINCHANNEL "441"
# define ERR_NOTONCHANNEL "442"
# define ERR_USERONCHANNEL "443"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_ALREADYREGISTERED "462"
# define ERR_PASSWDMISMATCH "464"
# define ERR_CHANNELISFULL "471"
# define ERR_INVITEONLYCHAN "473"
# define ERR_BADCHANNELKEY "475"
# define ERR_BADCHANMASK "476"
# define ERR_NOPRIVILEGES "481"
# define ERR_CHANOPRIVSNEEDED "482"
# define ERR_NOOPERHOST "491"
# define ERR_UMODEUNKNOWNFLAG "501"
# define ERR_USERSDONTMATCH "502"
# define ERR_HELPNOTFOUND "524"
# define ERR_INVALIDKEY "525"
# define ERR_INVALIDMODEPARAM "696"

#endif
