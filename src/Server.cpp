/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:47 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/16 16:01:47 by frlorenz         ###   ########.fr       */
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
	freeaddrinfo(_addrLst); // MUY IMPORTANTE: liberar la lista, es memoria reservada dinámicamente
}

void Server::parse_input(std::string buf)
{
	if (buf.find("REG") == 0)
		std::cout<<"Registration started, please enter: password, nickname and name"<<std::endl;
	if (buf.find("PASS") == 0)
	{
		if (sscanf((buf.c_str()), "PASS %s", (char *)_password.c_str()) != 1)
			std::cout<<"Wrong password"<<std::endl;
		else
			std::cout<<"Password accepted: "<<_password<<std::endl;
	}
	if (buf.find("NICK") == 0)
	{
		// std::cout<<buf;
		buf = buf.substr(5, buf.size());
		std::cout<< buf;
	}
}

void func ( int sigaction )
{
	(void)sigaction;
	
}

void Server::pollLoop()
{
	signal(SIGINT, func );
	signal(SIGPIPE, SIG_IGN);
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
		for (size_t i = 0; i < _pfd_arr.size(); ++i)
		{
			if (_pfd_arr[i].revents & POLLIN)
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
					else if (client_fd >= 0)
					{
						_pfd_arr.push_back((pollfd){client_fd, POLLIN, 0});
					}
				}
				else
				{
					//manejar cuando algun fd de cliente tiene algo que decir
					try
					{
						char buffer[256] = { 0 };
						ssize_t n_bytes = recv(_pfd_arr[i].fd, buffer, sizeof(buffer), 0);
						if (n_bytes < 0)
						{
							std::cerr << "recv error: " << strerror(errno) << std::endl;
						}
						else if (n_bytes == 0)
						{
							std::cout << "Client on socket " << _pfd_arr[i].fd << std::endl;
							_pfd_arr.erase(_pfd_arr.begin() + i);
						}
						else if (n_bytes > 0)
						{
							std::cout<<buffer;
							parse_input(buffer);
						}
					}
					catch(const std::exception& e)
					{
						std::cerr << "There was an error on socket " << _pfd_arr[i].fd << std::endl;
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
