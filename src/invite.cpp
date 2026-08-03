#include "Server.hpp"
#include "replies.hpp"

void cmdInvite(Server &s, Client &client, std::string &line)
{
    std::istringstream istr_line(line);
    std::string nick;
    std::string ch_name;

    istr_line >> nick >> ch_name;

    if (nick.empty() || ch_name.empty())
        return client.MsgToMe(ERR_NEEDMOREPARAMS(client.getNick(), client.getClientCmd()));

    if (!s.findClientbyNick(nick))
        return client.MsgToMe(ERR_NOSUCHNICK(client.getNick(), nick));

    if (!s.getChanel(ch_name))
        return client.MsgToMe(ERR_NOSUCHCHANNEL(client.getNick(), ch_name));

    Chanel *ch = s.getChanel(ch_name);
    if (!ch->isGuest(client.getFd()))
        return client.MsgToMe(ERR_CANNOTSENDTOCHAN(client.getNick(), ch_name));

    if (ch->isModed('i') && !ch->isAdmin(client.getFd()))
        return client.MsgToMe(ERR_CHANOPRIVSNEEDED(client.getNick(), ch_name));

    Client &to_invite = *(s.getClientbyNickname(nick));
    if (ch->isGuest(to_invite.getFd()))
        return client.MsgToMe(ERR_USERONCHANNEL(client.getNick(), nick, ch_name));

    ch->addGuest(to_invite.getFd());
    client.MsgToMe(RPL_INVITING(client.getNick(), to_invite.getNick(), ch_name));
    to_invite.MsgToMe(CMD_INVITE(client.getNick(), client.getName(), client.getHost(), to_invite.getNick(), ch_name));
}