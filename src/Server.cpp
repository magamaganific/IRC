/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgargantilla <dgargantilla@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:47 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/22 18:07:46 by frlorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::Server()
{}
Server::Server(char *port, char *password) : _port(port), _password(password), _serv_socket(-1), _addrLst(NULL)
{}

Server::Server(const Server &other)
{
	(void)other;
}

Server &Server::operator=(const Server &other)
{
	(void)other;
	return (*this);
}

std::string Server::get_port()
{
	return (this->_port);
}

Server::~Server()
{}

Chanel *Server::getChanel(std::string name)
{
    for (std::map<std::string, Chanel *>::iterator it = _chanels.begin(); it != _chanels.end(); ++it)
    {
        std::string tester = it->first;
        str_tolower(name);
        str_tolower(tester);
        if(tester == name)
            return (it->second);
    } 
    return(NULL);
}
std::map<std::string, Chanel *> &Server::getChanelsVector()
{
    return _chanels;
}

bool Server::findChanel(std::string name)
{
    for (std::map<std::string, Chanel *>::iterator it = _chanels.begin(); it != _chanels.end(); ++it)
    {
        std::string tester = it->first;
        str_tolower(name);
        str_tolower(tester);
        if(tester == name)
            return (true);
    } 
    return(false);
}


void Server::init()
{
	struct addrinfo	hints;
	int				status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, _port.c_str(), &hints, &_addrLst);
	if(status != 0)
		throw std::runtime_error("Status Error: " + std::string(gai_strerror(status)));
	_serv_socket = socket(_addrLst->ai_family, _addrLst->ai_socktype, _addrLst->ai_protocol);
	if (_serv_socket < 0)
		throw std::runtime_error("_Serv_Socket Error: " + std::string(strerror(errno)));
	int yes = 1;
	if (setsockopt(_serv_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		throw std::runtime_error("Error setsock: " + std::string(strerror(errno)));
	if (bind(_serv_socket, _addrLst->ai_addr, _addrLst->ai_addrlen) < 0){
		end();
        throw std::runtime_error("Error bind: " + std::string(strerror(errno)));
	}

	struct pollfd serv_pfd = { _serv_socket, POLLIN, 0 };
    _pfd_arr.push_back(serv_pfd);

    if (listen(_serv_socket, SOMAXCONN) < 0)
		throw std::runtime_error("Error listen: " + std::string(strerror(errno)));
		
	std::cout << my_serv_name"Server listening on port " << _port.c_str() << std::endl;
}

bool find_space(std::string buf)
{
	for (size_t i = 0; i < buf.size(); i++)
	{
		if (buf[i] == 32)
			return (true);
	}
	return (false);
}

bool Server::nick_is_valid(std::string buf, Client &client)
{
	if (!buf.size())
		return(client.MsgToMe(ERR_NONICKNAMEGIVEN(client.getName())), false);
	// std::cout<<"checkpoint"<<std::endl;
	if (buf[0] == '#' || buf[0] == ':' || find_space(buf))
		return(client.MsgToMe(ERR_ERRONEUSNICKNAME(client.getName(), buf)), false);
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (buf == it->second.getNick())
			return (client.MsgToMe(ERR_NICKNAMEINUSE(client.getName(), buf)), false);
	}
	return(true);
}

bool Server::cmdUser(Client &client, std::string buf){
	std::string username;
	std::string zero;
	std::string asterisk;
	std::string realname;
	std::string check;
	size_t pos;

	for (int i = 0; i < 4; i++)
	{
		pos = buf.find(' ');
		if (!pos)
			break;
		if (!username.size()){
			username = buf.substr(0, pos);
			if (username == "0" || username == "*" || username.size() < 1)
				return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getName(), "USER")), false);
		}
		else if (!zero.size()){
			zero = buf.substr(0, pos);
			if(zero != "0")
				return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getName(), "USER")), false);
		}
		else if (!asterisk.size()){
			asterisk = buf.substr(0, pos);
			if(asterisk != "*")
				return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getName(), "USER")),false);
		}
		else if (!realname.size())
			realname = buf;
		check = buf.substr(pos + 1);
		if (check == buf && !realname.size())
			return (client.MsgToMe(ERR_NEEDMOREPARAMS(client.getName(), "USER")), false);
		buf = check;
	}
	client.setName(username);
	client.setReal(realname);
	if (client.getNick() == "")
	{
		int i = 0;
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (username == it->second.getNick()){
				i = 1;
				client.MsgToMe(my_serv_name"Couldn't set Username to Nickname, please introduce a nickname");
				break;
			}
		}
		if (i == 0)
			client.setNick(username);
	}
	return(client.MsgToMe("Registration complete..."), true);
	
}

bool setup_complete(Client &client)
{
	if (client.getIsAuthenticated() == false)
		return(client.MsgToMe(my_serv_name" CSTOM " + client.getName() 
			+ " :Authenticate to access channel functions"), false);
	if (client.getIsRegistered() == false)
		return(client.MsgToMe(my_serv_name" CSTOM " + client.getName()
			+ " :Register to access channel functions"), false);
	return (true);
}

void Server::parse_input(Client &client)
{
	std::string buf = client.getBuf();
	// std::cout<<"Client buf: "<<buf<<std::endl;
	if (buf.find("CAP ") == 0)
		client.MsgToMe("No capabilities available.");
	else if (buf.find("PASS ") == 0)
	{
		buf = buf.substr(5, buf.size());
		if (client.getIsRegistered() == true)
			client.MsgToMe(ERR_ALREADYREGISTERED(client.getName()));
		if (buf != _password)
			client.MsgToMe(ERR_PASSWDMISMATCH(client.getName()));
		else
		{
			client.MsgToMe(RPL_WELCOME(client.getName()));
			client.setIsAuthenticated(true);
		}
	}
	else if (buf.find("NICK ") == 0)
	{
		buf = buf.substr(5, buf.size());
		if (nick_is_valid(buf, client)){
			client.setNick(buf);}
	}
	else if (buf.find("USER ") == 0)
	{
		if (client.getIsRegistered() == true)
			client.MsgToMe(ERR_ALREADYREGISTERED(client.getName()));
		else
			if (cmdUser(client, buf.substr(5, buf.size())))
				client.setIsRegistered(true);
	}
	else if (setup_complete(client)){
		if (buf.find("JOIN ") == 0)
			cmdJoin(*this, client, buf.substr(5, buf.size()));
		else if (buf.find("PRIVMSG ") == 0)
			cmdPrivmsg(*this, client, buf.substr(8, buf.size())); // cambiar por la funcion adecuada 
		else if (buf.find("QUIT ") == 0)
			buf = buf.substr(5, buf.size()); // cambiar por la funcion adecuada
		else if (buf.find("KICK ") == 0)
			buf = buf.substr(5, buf.size()); // cambiar por la funcion adecuada
		else if (buf.find("INVITE ") == 0)
			buf = buf.substr(7, buf.size()); // cambiar por la funcion adecuada
		else if (buf.find("TOPIC ") == 0)
			buf = buf.substr(6, buf.size()); // cambiar por la funcion adecuada
		else if (buf.find("MODE ") == 0)
			buf = buf.substr(5, buf.size()); // cambiar por la funcion adecuada
	}
	else
		client.MsgToMe(ERR_UNKNOWNCOMMAND(client.getName(), buf.substr(0, buf.find(" "))));
}

void Server::readClientInput(int fd, int i)
{
	char buf[256] = {'\0'};
	int nbytes = recv(fd, &buf, sizeof(buf), 0);

	// std::cout<<"nbytes: "<<nbytes<<std::endl;
	// std::cout<<"cli.getFd(): "<<_clients[fd].getFd()<<std::endl;
	if (nbytes <= 0){
		if (nbytes == 0){
			std::cout<<"Client "<<_clients[fd].getFd()<<" hung up"<<std::endl;
			_pfd_arr.erase(_pfd_arr.begin() + i);
			_disconnected_sockets.push_back(_clients[fd].getFd());
		}
		else
			throw std::runtime_error("recv error: " + std::string(strerror(errno)));
	}
	else
	{
		std::cout<<"original buffer:"<<buf;
		_clients[fd].setBuf(buf);
		size_t pos;
		if ((pos = _clients[fd].getBuf().find("\r\n")) != std::string::npos){
			while ((pos = _clients[fd].getBuf().find("\r\n")) != std::string::npos){
				std::string line = _clients[fd].getBuf().substr(0, pos);
				_clients[fd].getBuf().erase(0, pos + 2);
				_clients[fd].setBuf((char *)line.c_str());
				parse_input(_clients[fd]);
			}
		}
		else if ((pos = _clients[fd].getBuf().find("\n")) != std::string::npos){
			while ((pos = _clients[fd].getBuf().find("\n")) != std::string::npos){
				std::string line = _clients[fd].getBuf().substr(0, pos);
				_clients[fd].getBuf().erase(0, pos + 1);
				_clients[fd].setBuf((char *)line.c_str());
				parse_input(_clients[fd]);
			}
		}
		else
			parse_input(_clients[fd]);
		// std::cout<<"NICK: "<<_clients[fd].getNick()<<std::endl;
		// std::cout<<"USERNAME: "<<_clients[fd].getName()<<std::endl;
		// std::cout<<"REALNAME: "<<_clients[fd].getReal()<<std::endl;
		// std::cout<<"HOSTNAME: "<<_clients[fd].getHost()<<std::endl;
	}
}

void Server::accept_clients()
{
	// nueva conexión entrante -> accept()
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	int client_fd = accept(_serv_socket, (struct sockaddr*)&client_addr, &len);
	if (client_fd < 0)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			std::cerr << "accept error: " << strerror(errno) << std::endl;
		return;
	}
	else if (client_fd >= 0)
	{
		_acepted_fds.push_back(client_fd);

		std::cout << "Nueva Conexion" << std::endl;
	}
}

void Server::add_clients()
{
    for (size_t i = 0; i < _acepted_fds.size(); i++)
    {
        int clifd = _acepted_fds[i];
        struct pollfd pfd = { clifd, POLLIN, 0 };
        _pfd_arr.push_back(pfd);
        _clients.insert(std::make_pair(clifd, Client(clifd)));
    }
    _acepted_fds.clear();

}


void Server::disconnect_clients()
{
	  for (size_t i = 0; i < _disconnected_sockets.size(); i++)
    {
        std::vector<struct pollfd>::iterator it = _pfd_arr.begin();
        while (it != _pfd_arr.end())
        {
            if (_disconnected_sockets[i] == it->fd)
            {
                close(it->fd);
                _clients.erase(it -> fd);
                it = _pfd_arr.erase(it);
                break;
            }
            it++;
        }
    }
    _disconnected_sockets.clear();
}

void Server::pollLoop()
{
	while (true)
	{
		std::cout << _pfd_arr.size() - 1 << " connected clients. Waiting for events..." << std::endl;
		int ready = poll(&_pfd_arr[0], _pfd_arr.size(), -1); // -1 = espera indefinida
		if (ready < 0) /* manejar error, ojo con EINTR */ 
		{
			if (errno != EINTR)
				std::cerr << "poll error: " << strerror(errno) << std::endl;
			break;
		}
		for (size_t i = 0; i < _pfd_arr.size(); i++)
		{
			if (_pfd_arr[i].revents & (POLLIN))
			{
				if (_pfd_arr[i].fd == _serv_socket) // si nosotros somos el listener, es una nueva conexion
				{
					accept_clients();
				}
				else
				{
					try
					{
						readClientInput(_pfd_arr[i].fd, i);
					}
					catch(const std::exception& e)
					{
						std::cerr << "There was an error on socket " << _pfd_arr[i].fd << std::endl;
						_disconnected_sockets.push_back(_pfd_arr[i].fd);
					}
					
				}
			}
			else if (_pfd_arr[i].revents & (POLLERR | POLLNVAL  | POLLHUP))
            {
                std::cerr << "There was an error on socket " << _pfd_arr[i].fd << std::endl;
				// _pfd_arr.erase(_pfd_arr.begin() + i);
				_disconnected_sockets.push_back(_pfd_arr[i].fd);
				//desconectar y cerrar lo que corresponda
            }
			
		}
		if (_acepted_fds.size() > 0)
			add_clients();
		if (_disconnected_sockets.size() > 0)
			disconnect_clients();
	}
}

//Esta es la funcion que manda al fd que le pases el msg <std::string>
void Server::SendMsg(int fd, std::string msg)
{
    if (msg.length() > 510)
        msg.erase(510);
    std::cout << fd << " " << msg << "\n";
    msg += "\r\n";

    ssize_t total = 0;
    ssize_t length = static_cast<ssize_t>(msg.size());
    while (total < length)
    {
        ssize_t n_bytes = send(fd, msg.c_str() + total, length - total, 0);
        if (n_bytes < 0)
            throw std::runtime_error(strerror(errno));
        total += n_bytes;
    }
}

void Server::end(){
	freeaddrinfo(_addrLst);
}
