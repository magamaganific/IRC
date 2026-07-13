/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:36 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/13 16:52:44 by frlorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
 #define SERVER_HPP

# include <iostream>
# include <cstring>
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


class Server
{
	private:
		const std::string			_port;
		std::string					_password;
        int							_serv_socket;
        struct addrinfo				*_addrLst; // lista enlazada de `sockaddr` de algún tipo que podremos utilizar más adelante
		std::vector<struct pollfd>	_pfd_arr;
		
	public:
		Server();
		Server(char *port, char *password);
		Server(const Server &other);
		Server &operator=(const Server &other);
		std::string get_port();
		void init();
		void pollLoop();
		~Server();
};

#endif