#include "Server.hpp"
#include "Chanel.hpp"
#include "Client.hpp"
#include "replies.hpp"


void TypeB(Server &s, Chanel &chan, Client &c, char mode_str, bool set, std::istringstream &str)
{
    std::string args;
    str >> args;
    if (args.empty())
        return;
    if (mode_str == 'o')
    {
        if (!s.findClientbyNick(args))
            return c.MsgToMe(ERR_NOSUCHNICK(c.getNick(), args));
        Client &cli_to_mode = *s.getClientbyNick(args);
        if(!chan.isMember(cli_to_mode.getFd()))
            return c.MsgToMe(ERR_USERNOTINCHANNEL(c.getNick(),chan.getChanelName()));
        bool is_admin = chan.isAdmin(cli_to_mode.getFd());
        if ((!set && !is_admin) || (set && is_admin))
            return;
        if (!set)
            chan.removeFromAdmins(s, cli_to_mode.getFd());
        else
            chan.addAdmin(cli_to_mode.getFd());
    }
}


void TypeC(Chanel &chan, char mode_str, bool set, std::istringstream &str)
{
    if (set)
    {
        if (!chan.isModed(mode_str))
            return;
        chan.unsetMode(mode_str);
    }
    else if (!set)
    {
        std::string args;
        str >> args;
        if (args.empty())
            return;
        if (mode_str == 'l')
        {
            int val;
            std::istringstream iss(args);
            iss >> val;
            if (iss.fail() || !iss.eof())
                return;
            if (val <= 0 || chan.getLimit() == val)
                return;
            chan.setMode('l');
            chan.setLimit(val);
        }
        else if (mode_str == 'k')
        {
            if (chan.getChanelPass() == args)
                return;
            chan.setMode('k');
            chan.setChanelPass(args);
        }
    }
}


void TypeD(Chanel &chan, char mode_str, bool set)
{
    if (!set)
    {
        if (!chan.isModed(mode_str))
            return;
        chan.unsetMode(mode_str);
    }
    else if (set)
    {
        if (chan.isModed(mode_str))
            return;
        chan.setMode(mode_str);
    }
}


void cmdMode(Server &s, Client &c, std::string &arg)
{
    std::istringstream str(arg);
    std::string to_mode;
    std::string mode_str;

    str >> to_mode >> mode_str;

    if (to_mode[0] != '#' && !mode_str.empty())
        return c.MsgToMe(ERR_UMODEUNKNOWNFLAG(c.getNick()));
    if (!s.findChanel(to_mode))
        return c.MsgToMe(ERR_NOSUCHCHANNEL(c.getNick(), to_mode));
    
    Chanel &chan = *(s.getChanel(to_mode));

    if (!chan.isAdmin(c.getFd()))
        return c.MsgToMe(ERR_CHANOPRIVSNEEDED(c.getNick(), to_mode));
    bool set = true;
    
    for (size_t i = 0; i < mode_str.length(); ++i)
    {
        if (mode_str[i] == '+' || mode_str[i] == '-')
        {
            if (mode_str[i] == '+')
                set = true;
            else
                set = false;
        }
        else if (mode_str[i] == 'o')
            TypeB(s, chan, c, mode_str[i], set, str);
        else if (mode_str[i] == 'k' || mode_str[i] == 'l')
            TypeC(chan, mode_str[i], set, str);
        else if (mode_str[i] == 'i' || mode_str[i] == 't')
            TypeD(chan, mode_str[i], set);
        else
            c.MsgToMe(ERR_UNKNOWNMODE(c.getNick(), mode_str[i]));
    }
}