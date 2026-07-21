
#ifndef CLIENT_HPP
 #define CLIENT_HPP
 #include <string>

class Client
{
private:
	int _fd;
	std::string _nick;
	std::string _name;
	bool		_isRegistered;
	bool		_isAuthenticated;
	std::string _buf;
public:
	Client();
	Client(int fd);
	Client(const Client &old);
	Client&operator=(const Client &old);
	~Client();

	void setName(std::string name);
	void setNick(std::string nick);
	void setBuf(char *buf);
	void setIsRegistered(bool tof);
	void setIsAuthenticated(bool tof);

	std::string getName();
	std::string getNick();
	std::string getBuf();
	bool		getIsRegistered();
	bool		getIsAuthenticated();
	int getFd() const;
};

#endif