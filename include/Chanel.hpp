#ifndef CHANEL_HPP
# define CHANEL_HPP

# include "Server.hpp"
# include "Client.hpp"

class Chanel
{
private:
	/* atributos */
	// chanel_name
	std::string         _chanel_name;
	// topic
	std::string         _topic;
	// password
	std::string         _chanel_pass;
	// chanel_type (si es tipo invite solo puedes entrar por invitacion)
	// limite de usuarios (limitado o indefinido)
	int				_limit;
	// array de admins
	std::vector<int>    _admins;
	// array de usuaros (presentes en el canal)
	std::vector<int>    _members;
	// array de invitados
	std::vector<int>    _guests;
public:
	Chanel();
	Chanel(std::string name, std::string pass, int admin);
	Chanel(const Chanel &object);
	Chanel &operator=(const Chanel &object);
    
	const std::string &getChanelName() const;
    const std::string &getChanelTopic() const;
	const std::string &getChanelPass() const;
	const int &getLimit() const;
	const std::vector<int> &getChanelAdmins() const;
	const std::vector<int> &getChanelMembers() const;
    
	void setChanelName(const std::string name);
    void setChanelTopic(const std::string topic);
	void setChanelPass(const std::string pass);
	void addMember(int fd);
	void addAdmin(int fd);
	void addGuest(int fd);
	~Chanel();

	
};

#endif