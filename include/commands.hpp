#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"
#include "utils.hpp"

class Server;
class Chanel;
class Client;

void cmdJoin(Server &s, Client& client, std::string line);