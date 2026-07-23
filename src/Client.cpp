#include "../include/Client.hpp"
#include "../include/Server.hpp"


Client::Client(){
}

Client::Client(int fd)
:_fd(fd), _nick(""), _name(""),
_realname(""), _hostname(""),
_isRegistered(false), _isAuthenticated(false),
_isOperator(false), _buf(""){
}

Client::Client(const Client &old)
: _fd(old._fd), _nick(old._nick), _name(old._name),
_realname(old._realname), _hostname(old._hostname),
_isRegistered(old._isRegistered), _isAuthenticated(old._isAuthenticated),
_isOperator(old._isOperator), _buf(old._buf){
}

Client &Client::operator=(const Client &old){
	if (this != &old)
	{
		this->_fd = old._fd;
		this->_nick = old._nick;
		this->_name = old._name;
		this->_realname = old._realname;
		this->_hostname = old._hostname;
		this->_isRegistered = old._isRegistered;
		this->_isAuthenticated = old._isAuthenticated;
		this->_isOperator = old._isOperator;
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

void Client::setReal(std::string realname){
	this->_realname = realname;
}

void Client::setHost(std::string hostname){
	this->_hostname = hostname;
}

void Client::setBuf(char *buf){
	this->_buf = buf;
}

void Client::setIsRegistered(bool tof){
	_isRegistered = tof;
}

void Client::setIsAuthenticated(bool tof){
	_isAuthenticated = tof;
}

void Client::setIsOperator(bool tof){
	_isOperator = tof;
}

std::string Client::getName(){
	return(_name);
}

std::string Client::getNick(){
	return(_nick);
}

std::string Client::getReal(){
	return(_realname);
}

std::string Client::getHost(){
	return(_hostname);
}

std::string Client::getBuf(){
	return(_buf);
}

bool Client::getIsRegistered(){
	return(_isRegistered);
}

bool Client::getIsAuthenticated(){
	return(_isAuthenticated);
}

bool Client::getIsOperator(){
	return(_isOperator);
}


int Client::getFd() const{
	// std::cout<<"get->"<<this->_fd<<std::endl;
	return(_fd);
}