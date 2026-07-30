/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
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
		throw std::runtime_error("Error: " + std::string(gai_strerror(status)));
	_serv_socket = socket(_addrLst->ai_family, _addrLst->ai_socktype, _addrLst->ai_protocol);
	if (_serv_socket < 0)
		throw std::runtime_error("Error: " + std::string(strerror(errno)));
	int yes = 1;
	if (setsockopt(_serv_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		throw std::runtime_error("Error setsock: " + std::string(strerror(errno)));
	if (bind(_serv_socket, _addrLst->ai_addr, _addrLst->ai_addrlen) < 0)
        throw std::runtime_error("Error bind: " + std::string(strerror(errno)));

	struct pollfd serv_pfd = { _serv_socket, POLLIN, 0 };
    _pfd_arr.push_back(serv_pfd);

    if (listen(_serv_socket, SOMAXCONN) < 0)
		throw std::runtime_error("Error listen: " + std::string(strerror(errno)));
		
	std::cout << "Server listening on port " << _port.c_str() << std::endl;
	// MUY IMPORTANTE: liberar la lista, es memoria reservada dinámicamente:
	//freeaddrinfo(_addrLst);
}

void Server::parse_input(Client &client)
{
	std::string buf = client.getBuf();
	if (buf.find("CAP") == 0)
		std::cout<<"No capabilities available"<<std::endl;
	if (buf.find("PASS") == 0)
	{
		std::cout<<"buff: "<<buf.size()<<std::endl;
		buf = buf.substr(5, buf.size());
		std::cout<<"buff: "<<buf<<std::endl;
		std::cout<<"buff: "<<buf.size()<<std::endl;
		std::cout<<"pass: "<<_password<<std::endl;
		if (buf != _password)
			std::cout<<"Wrong password"<<std::endl;
		else
		{
			std::cout<<"Password accepted: "<<_password<<std::endl;
			client.setIsAuthenticated(true);
		}
	}
	if (buf.find("NICK") == 0)
	{
		if (client.getIsAuthenticated() == false){
			std::cout<<"You are not authenticated, Please introduce the server password"<<std::endl;
			//send it to the client
		}
		else{
			buf = buf.substr(5, buf.size());
			client.setNick(buf);
			client.setIsRegistered(true);
		}
	}
	if (buf.find("USER") == 0)
	{
		if (client.getIsAuthenticated() == false){
			std::cout<<"You are not authenticated, Please introduce the server password"<<std::endl;
			//send it to the client
		}
		else{
			buf = buf.substr(6, buf.size());
			client.setName(buf);
			client.setIsRegistered(true);
		}
	}
}

void Server::readClientInput(int fd, int i)
{
	char buf[256] = {'\0'};
	// Client &cli = _clients[fd];
	int nbytes = recv(fd, &buf, sizeof(buf), 0);

	// std::cout<<"nbytes: "<<nbytes<<std::endl;
	// std::cout<<"cli.getFd(): "<<cli.getFd()<<std::endl;
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
		std::cout<<buf;
		_clients[fd].setBuf(buf);
		size_t pos;
		while ((pos = _clients[fd].getBuf().find("\r\n")) != std::string::npos){
			std::string line = _clients[fd].getBuf().substr(0, pos);
			_clients[fd].getBuf().erase(0, pos + 2);
			_clients[fd].setBuf((char *)line.c_str());
			parse_input(_clients[fd]);
		}
		std::cout<<_clients[fd].getNick()<<std::endl;
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

		std::cout << "Nueva Conexión" << std::endl;
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