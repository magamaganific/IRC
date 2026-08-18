#include "Server.hpp"
#include "replies.hpp"

void cmdPing(Client &cli, std::string &token)
{
    if (token.empty())
        return cli.MsgToMe(ERR_NEEDMOREPARAMS(cli.getNick(), "PING"));
    
    cli.MsgToMe(CMD_PONG(token));
}