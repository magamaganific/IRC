#include "Chanel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"
#include "commands.hpp"

#include <sstream>

void cmdPrivmsg(Server &s, Client &client, std::string line)
{
	std::vector<std::string> sendto;
	std::string message = line;
	size_t pos = line.find(" :");
	size_t name_pos =  0;

	if (line.find(":") == 0 || pos == 0)
		return(client.MsgToMe(ERR_NORECIPIENT(client.getName(), "PRIVMSG")));
	if (pos == line.npos)
		return(client.MsgToMe(ERR_NOTEXTTOSEND(client.getName())));
	// std::cout<<line.npos<<std::endl;
	if (line.find(",") != line.npos && line.find(",") < pos){
		for (size_t i = 0; i < pos; i += name_pos)
		{
			name_pos = message.find(",");
			if (name_pos == message.npos || name_pos > pos)
				break;
			if (!message.substr(0, name_pos).size())
				return(client.MsgToMe(ERR_NORECIPIENT(client.getName(), "PRIVMSG")));
			sendto.push_back(message.substr(0, name_pos));
			message = message.substr(name_pos + 1);
		}
	}
	name_pos = message.find(" ");
	if (!message.substr(0, name_pos).size())
		return(client.MsgToMe(ERR_NORECIPIENT(client.getName(), "PRIVMSG")));
	sendto.push_back(message.substr(0, name_pos));
	std::cout<<sendto[0]<<std::endl;
	(void) s;
}