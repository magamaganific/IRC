
#ifndef CLIENT_HPP
 #define CLIENT_HPP
 
 #include <string>

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
public:
	Client();
	Client(int fd);
	Client(const Client &old);
	Client&operator=(const Client &old);
	~Client();

	void setName(std::string name);
	void setNick(std::string nick);
	void setReal(std::string realname);
	void setHost(std::string hostname);
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
	int getFd() const;
};

#endif