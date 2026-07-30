/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgargantilla <dgargantilla@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:36 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/22 17:59:33 by frlorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
 #define SERVER_HPP

# include <iostream>
# include <fstream>
# include <cstring>
# include <cstdio>
# include <sstream>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <poll.h>
# include <exception>
# include <errno.h>
# include <vector>
# include <map>
# include <unistd.h>
# include <string>
# include "Client.hpp"
# include "replies.hpp"
# include "Chanel.hpp"
# include "utils.hpp"


class Server
{
	private:
		const std::string					_port;
		std::string							_password;
        int									_serv_socket;
        struct addrinfo						*_addrLst; // lista enlazada de `sockaddr` de algún tipo que podremos utilizar más adelante
		std::vector<struct pollfd>			_pfd_arr;
		std::map<int, Client>				_clients;
		std::vector<int>					_acepted_fds;
		std::vector<int>					_disconnected_sockets;
		std::map<std::string, Chanel *>		_chanels;
		
	public:
		Server();
		Server(char *port, char *password);
		Server(const Server &other);
		Server &operator=(const Server &other);
		std::string get_port();
		void init();
		void end();
		void pollLoop();
		void accept_clients();
		void add_clients();
		void disconnect_clients();
		void readClientInput(int fd, int i);
		void parse_input(Client &client);
		bool nick_is_valid(std::string buf);
		bool parse_user_command(Client &client, std::string buf);
		bool findChanel(std::string name);
    	Chanel *getChanel(std::string name);
		std::map<std::string, Chanel *> &Server::getChanelsVector();
		void SendMsg(int fd, std::string msg);
		~Server();
};

#endif