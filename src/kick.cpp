#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"
#include "commands.hpp"
#include "utils.hpp"

std::string process_Comment(std::istringstream &iss)
{
    std::string comment;

    std::getline(iss, comment);
    if (!comment.empty() && comment.find(':') == std::string::npos)
        comment += ':';
    if (!comment.empty() && comment[0] == ' ')
        comment.erase(0, 1);
    if (!comment.empty() && comment[0] == ':')
        comment.erase(0, 1);
    while (!comment.empty() && comment[0] == ' ')
    {
        comment.erase(0, 1);
    }
    return (comment);
}

std::vector<std::string> ft_split(const std::string& text, char character) {
    std::vector<std::string> result;
    std::stringstream stream(text);
    std::string part;
    
    while (std::getline(stream, part, character)) {
        result.push_back(part);
    }
    
    return (result);
}

void cmdKick(Server &s, Client &client, std::string line)
{
	std::istringstream iss(line);
    std::string chanelName;
    std::string nick;
    std::string comment;
    iss >> chanelName >> nick;
    
    if (chanelName.empty() || nick.empty())
        return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getNick(), "KICK")));
    if(!s.findChanel(chanelName))
        return (client.MsgToMe(ERR_NOSUCHCHANNEL(client.getNick(), chanelName)));
    Chanel *chanel = s.getChanel(chanelName);
    if (!chanel->isMember(client.getFd()))
        return(client.MsgToMe(ERR_NOTONCHANNEL(client.getNick(), chanelName)));
    if (!chanel->isAdmin(client.getFd()))
        return(client.MsgToMe(ERR_CHANOPRIVSNEEDED(client.getNick(), chanelName)));
    comment = process_Comment(iss);
    if (comment.empty())
        comment = "Kicked out by an operator";
    else if (!comment.empty() && comment.find(':') != std::string::npos)
        return (client.MsgToMe(ERR_UNKNOWNERROR(client.getNick(), "KICK", "Could not process, multiple invalid parameters")));

    std::vector<std::string> nicks = ft_split(nick, ',');
    for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); ++it)
    {
        Client *member = s.getClientbyNick(*it);
        if (!member || !chanel->isMember(member->getFd()))
            client.MsgToMe(ERR_USERNOTINCHANNEL(*it, chanelName));
        else
        {
            std::string msgKick = ":" + client.getNick() + "!" + client.getName() + "@" + client.getHost() + " KICK " + chanelName + " " + *it + " :" + comment;
            chanel->sendMsgToMembers(&s, msgKick);
            chanel->removeClient(s, member->getFd());
            member->deleteChanel(*chanel);
            s.eraseChanel(chanel);
        }
    }
}