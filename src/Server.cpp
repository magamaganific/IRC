/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:47 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/13 18:00:43 by frlorenz         ###   ########.fr       */
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
	
	//!!ESTO NO VA AQUI PERO POR AHORA ES DONDE FUNCIONA!!!
	//freeaddrinfo(_addrLst); // MUY IMPORTANTE: liberar la lista, es memoria reservada dinámicamente
}

void Server::readClientInput(int fd)
{
	char buf[256];
	Client &cli = _clients[fd];
	int nbytes = recv(fd, &buf, sizeof(buf), 0);

	std::cout<<nbytes<<std::endl;
	std::cout<<cli.getFd()<<std::endl;
	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout<<"Client %d hung up"<< cli.getFd()<<std::endl;
		else
			throw std::runtime_error("recv error: " + std::string(strerror(errno)));
	}
	else
	{
		std::cout<<buf<<std::endl;
		std::cout<<"something else"<<std::endl;
	}
}

void Server::pollLoop()
{
	int j = 0;
	while (j == 0)
	{
		int ready = poll(&_pfd_arr[0], _pfd_arr.size(), -1); // -1 = espera indefinida
		if (ready < 0) /* manejar error, ojo con EINTR */ 
		{
			if (errno != EINTR)
				std::cerr << "poll error: " << strerror(errno) << std::endl;
			break;
		}
		for (size_t i = 0; i < _pfd_arr.size(); ++i)
		{
			if (_pfd_arr[i].revents & (POLLIN | POLLHUP))
			{
				if (_pfd_arr[i].fd == _serv_socket) // si nosotros somos el listener, es una nueva conexion
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
					std::cout << "Viene otro!!" << std::endl;
					Client client(client_fd);
					this->_clients.insert(std::make_pair(client_fd, client));
					_pfd_arr.push_back((pollfd){client_fd, POLLIN, 0});
					std::cout<<client_fd<<std::endl;
					std::cout<<client.getFd()<<std::endl<<std::endl;
				}
				else
				{
					try
					{
						readClientInput(_pfd_arr[i].fd);
						// datos de un cliente existente -> recv()
						std::cout << "ESAMOS ESCRIBIENDO" << std::endl;
					}
					catch(const std::exception& e)
					{
						std::cerr << "There was an error on socket " << _pfd_arr[i].fd << std::endl;
						j = 1;
					}
					
				}
			}
			else if (_pfd_arr[i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                std::cerr << "There was an error on socket " << _pfd_arr[i].fd << std::endl;
				//desconectar y cerrar lo que corresponda
            }
		}
	}
}

void Server::end(){
	freeaddrinfo(_addrLst);
}