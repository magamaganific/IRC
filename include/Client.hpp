
#ifndef CLIENT_HPP
 #define CLIENT_HPP
 #include <string>

class Client
{
private:
	std::string _name;
	std::string _nick;
	int _fd;
public:
	// Client();
	Client(int fd);
	Client(const Client &old);
	Client&operator=(const Client &old);
	~Client();

	void setName(std::string name);
	void setNick(std::string nick);

	std::string getName();
	std::string getNick();
	int getFd();
};

#endif