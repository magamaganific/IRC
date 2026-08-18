#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"
#include "utils.hpp"

class Server;
class Chanel;
class Client;

void cmdJoin(Server &s, Client& client, std::string line);
void cmdPrivmsg(Server &s, Client &client, std::string line);
void cmdTopic(Server &s, Client &client, std::string line);
void cmdInvite(Server &s, Client &client, std::string line);
void cmdMode(Server &s, Client &c, std::string arg);
void cmdKick(Server &s, Client &client, std::string line);
void cmdPing(Client &cli, std::string token);