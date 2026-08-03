#ifndef CLIENT_HPP
 #define CLIENT_HPP
 
 #include "Server.hpp"
 #include "Chanel.hpp"

 class Chanel;

class Client
{
private:
	int _fd;
	std::string _nick;
	std::string _name;
	std::string _realname;
	std::string _hostname;
	bool		_isRegistered;
	bool		_isAuthenticated;
	bool		_isOperator;
	std::string _buf;
	std::vector<std::string>    _chanels;
public:
	Client();
	Client(int fd);
	Client(const Client &old);
	Client&operator=(const Client &old);
	~Client();

	void setName(std::string name);
	void setNick(std::string nick);
	void setReal(std::string real);
	void setHost(std::string host);
	void setBuf(char *buf);
	void setIsRegistered(bool tof);
	void setIsAuthenticated(bool tof);
	void setIsOperator(bool tof);

	std::string getName();
	std::string getNick();
	std::string getReal();
	std::string getHost();
	std::string getBuf();
	bool		getIsRegistered();
	bool		getIsAuthenticated();
	bool		getIsOperator();
	void 		addChanel(const Chanel& ch);
  	void 		deleteChanel(const Chanel& ch);
	const std::vector<std::string> &getChanels() const;
	void MsgToMe(std::string msg);
	
	int getFd() const;

	void MsgToMe(std::string msg);
};

#endif