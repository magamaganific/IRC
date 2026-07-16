#include "../include/Client.hpp"
#include "../include/Server.hpp"


Client::Client(){
}

Client::Client(int fd):_fd(fd), _nick(""), _name(""){
}

Client::Client(const Client &old): _fd(old._fd), _nick(old._nick), _name(old._name){
}

Client &Client::operator=(const Client &old){
	if (this != &old)
	{
		this->_fd = old._fd;
		this->_nick = old._nick;
		this->_name = old._name;
	}
	return (*this);
}

Client::~Client(){}

void Client::setName(std::string name){
	this->_name = name;
}

void Client::setNick(std::string nick){
	this->_nick = nick;
}

std::string Client::getName(){
	return(this->_name);
}

std::string Client::getNick(){
	return(this->_nick);
}

int Client::getFd() const{
	std::cout<<"get->"<<this->_fd<<std::endl;
	return(_fd);
}