#include "../include/Client.hpp"
#include "../include/Server.hpp"


Client::Client(){
}

Client::Client(int fd)
:_fd(fd), _nick(""), _name(""),
_isRegistered(false), _isAuthenticated(false),
_buf(""){
}

Client::Client(const Client &old)
: _fd(old._fd), _nick(old._nick), _name(old._name),
_isRegistered(old._isRegistered), _isAuthenticated(old._isAuthenticated),
_buf(old._buf){
}

Client &Client::operator=(const Client &old){
	if (this != &old)
	{
		this->_fd = old._fd;
		this->_nick = old._nick;
		this->_name = old._name;
		this->_isRegistered = old._isRegistered;
		this->_isAuthenticated = old._isAuthenticated;
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

void Client::setBuf(char *buf){
	this->_buf = buf;
}

std::string Client::getName(){
	return(_name);
}

std::string Client::getNick(){
	return(_nick);
}

std::string Client::getBuf(){
	return(_buf);
}

bool Client::getIsRegistered(){
	return(_isRegistered);
}

int Client::getFd() const{
	// std::cout<<"get->"<<this->_fd<<std::endl;
	return(_fd);
}