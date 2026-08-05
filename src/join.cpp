#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"

#include <sstream>


bool validPass(Chanel* chanel, std::string pass, int fd)
{
    if(chanel->isModed('k'))
    {
        if(chanel->getChanelPass().empty())
            return true;
        else if((chanel->getChanelPass() == pass) || (chanel->isGuest(fd)))
        {
            /*if (chanel->isGuest(fd))
                chanel->removeFromGuests(fd);*/
            return true;
        }
        else
            return false;
    }
    return(true);
}

bool validCapacity(Chanel *chanel)
{
    if(chanel->isModed('l') && chanel->getLimit() > 0)
    {
        if(chanel->getChanelMembers().size() < (size_t)chanel->getLimit())
            return(true);
        else
            return(false);
    }
    else
        return(true);
}

bool isPrivate(Chanel *chanel, int fd)
{
    if(chanel->isModed('i'))
    {
        if(chanel->isGuest(fd))
            return(true);
        else
            return(false);
    }
    else
        return(true);
}

bool nameIsValid(std::string name)
{
    if(name[0] != '#' && name[0] != '&')
        return false;
    else if (name.size() == 1)
        return false;
    return(true);
}

std::map<std::string, std::string> getChanelParams(std::string names, std::string pass, Client &client)
{
    std::istringstream  name_list(names);
    std::istringstream  pass_list(pass);
    std::string         final_names;
    std::string         final_pass;
    std::string         nick = client.getNick();
    std::map<std::string, std::string> ch_params;

    while(std::getline(name_list, final_names, ','))
    {
        if(!std::getline(pass_list, final_pass, ','))
            final_pass = "";
        if(final_names[0] == '#' && final_names.size() == 1)
        {
            client.MsgToMe(ERR_NEEDMOREPARAMS(nick, client.getClientCmd()));
            return std::map<std::string, std::string> ();
        }
        if(!nameIsValid(final_names))
        {
            client.MsgToMe(ERR_BADCHANMASK(nick, final_names));
            return std::map<std::string, std::string> ();
        }
        ch_params[final_names] = final_pass;
    }
    return (ch_params);
}

void joinNotification(Server &s, Client& client, std::string ch_name)
{
    std::string joinMsg;
    std::string topic = s.getChanel(ch_name)->getChanelTopic();
    std::string nick = client.getNick();
    std::vector<int> clients = s.getChanel(ch_name)->getChanelMembers();

    joinMsg = ":" + client.getNick() + "!" + client.getName() + "@" + client.getHost()+ " JOIN " + ch_name;
    s.getChanel(ch_name)-> sendMsgToMembers(joinMsg);
    if(!topic.empty())
        client.MsgToMe(RPL_TOPIC(nick, ch_name, topic));
    else
        client.MsgToMe(RPL_NOTOPIC(nick, ch_name));
}


void cmdJoin(Server &s, Client& client, std::string line)
{
    std::istringstream  str(line);
    std::string         nick = client.getNick();
    std::string         name;
    std::string         pass;
    std::map <std::string, std::string> ch_params;

    str >> name;
    str >> pass;
    ch_params = getChanelParams(name, pass, client);
    
    for(std::map<std::string, std::string>::iterator iter = ch_params.begin(); iter != ch_params.end(); ++iter)
    {
        std::string f_name = iter->first;
        std::string f_pass = iter->second;
        if(s.findChanel(f_name))
        {
            Chanel *ch = s.getChanel(f_name);
            if (!validPass(ch, f_pass, client.getFd()))
                client.MsgToMe(ERR_BADCHANNELKEY(nick, f_name));
            else if (!validCapacity(ch))
                client.MsgToMe(ERR_CHANNELISFULL(nick, f_name));
            else if (!isPrivate(ch, client.getFd()))
                client.MsgToMe(ERR_INVITEONLYCHAN(nick, f_name));
            else
            {
                ch->addMember(client.getFd());
                client.addChanel(*(ch));
                joinNotification(s, client, name);
            }
        }
        else
        {
            Chanel *ch = new Chanel (f_name, f_pass, client.getFd());
            s.getChanelsVector()[f_name] = ch;
            client.addChanel(*ch);
            joinNotification(s, client, name);
        }
    }
}