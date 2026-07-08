/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:40:36 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/08 12:44:00 by frlorenz         ###   ########.fr       */
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
# include <exception>
# include <errno.h>


class Server
{
	private:
		const std::string	_port;
		std::string			_password;
        int					_serv_socket;
        struct addrinfo		*_addrLst; // lista enlazada de `sockaddr` de algún tipo que podremos utilizar más adelante
	
	public:
		Server();
		Server(char *port, char *password);
		Server(const Server &other);
		Server &operator=(const Server &other);
		std::string get_port();
		void init();
		~Server();
};

#endif