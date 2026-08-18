#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"
#include "commands.hpp"

#include <sstream>

void cmdTopic(Server &s, Client &client, std::string line)
{
	std::string channel;
	std::string newtop;
	
	if(!line.size())
		return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getNick(), "TOPIC")));
	if(line.find(" :") != line.npos)
	{
		if (!(line.find(" ") == line.find(" :")))
			return (client.MsgToMe("TOPIC only accepts 1 channel + topic."));
		channel = line.substr(0, line.find(" :"));
		newtop = line.substr(line.find(" :") + 2, line.size());
		if (!s.getChanel(channel))
			return (client.MsgToMe(ERR_NOSUCHCHANNEL(client.getNick(), channel)));
		if (!(s.getChanel(channel)->isMember(client.getFd())))
			return (client.MsgToMe(ERR_NOTONCHANNEL(client.getNick(), channel)));
		if (s.getChanel(channel)->isModed('t'))
		{
			if (!s.getChanel(channel)->isAdmin(client.getFd()))
				return (client.MsgToMe(ERR_CHANOPRIVSNEEDED(client.getNick(), channel)));
		}
		s.getChanel(channel)->setChanelTopic(newtop);
		if (s.getChanel(channel)->getChanelTopic().size())
			return(s.getChanel(channel)->sendMsgToMembers(&s, RPL_TOPIC(client.getNick(), channel, s.getChanel(channel)->getChanelTopic()), 0));
		return(s.getChanel(channel)->sendMsgToMembers(&s, RPL_NOTOPIC(client.getNick(), channel), client.getFd()));
	}
	else{
		channel = line;
		if (!s.getChanel(channel))
			return (client.MsgToMe(ERR_NOSUCHCHANNEL(client.getNick(), channel)));
		if (s.getChanel(channel)->getChanelTopic().size())
			return(client.MsgToMe(RPL_TOPIC(client.getNick(), channel, s.getChanel(channel)->getChanelTopic())));
		return(client.MsgToMe(RPL_NOTOPIC(client.getNick(), channel)));
	}
}