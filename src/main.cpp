/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frlorenz <frlorenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 12:24:38 by frlorenz          #+#    #+#             */
/*   Updated: 2026/07/21 15:15:20 by frlorenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int check_in(char **argv)
{
    std::stringstream ss(argv[1]);
    int port;
    std::string str;
    std::string pass(argv[2]);
    
    if (!(ss >> port) || ss >> str)
        return (0);
    if (port < 0 || port > 65535)
        return (0);
    
    if (!pass.empty())
    {
        for(size_t i = 0; i < pass.length(); i++)
        {
            if (pass[i] <= 32 || pass[i] > 127)
                return (0);
        }
    }else
        return (0);
    return (1);
}

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        return (1);
    }
	if (!check_in(argv))
    {
        std::cerr << "Error: invalid arguments." << std::endl;
        return (1);
    }
    
	try
    {
        Server server(argv[1], argv[2]);
        server.init();
        server.pollLoop();
        server.end();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
	return (0);
}