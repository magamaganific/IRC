
#ifndef CLIENT_HPP
 #define CLIENT_HPP
 #include <string>

class Client
{
private:
	int _fd;
	std::string _nick;
	std::string _name;
public:
	Client();
	Client(int fd);
	Client(const Client &old);
	Client&operator=(const Client &old);
	~Client();

	void setName(std::string name);
	void setNick(std::string nick);

	std::string getName();
	std::string getNick();
	int getFd() const;
};

#endif