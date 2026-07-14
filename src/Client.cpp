#include "../include/Client.hpp"

Client::Client(int fd): _fd(fd){
}

Client::Client(const Client &old){
	(void)old;
}

Client &Client::operator=(const Client &old){
	(void)old;
	return(*this);
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

int Client::getFd(){
	return(this->_fd);
}