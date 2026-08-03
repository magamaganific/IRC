#include "Client.hpp"

Client::Client(){
}

Client::Client(int fd)
:_fd(fd), _nick(""), _name(""),
_isRegistered(false), _isAuthenticated(false),
_isOperator(false), _buf(""){
}

Client::Client(const Client &old)
: _fd(old._fd), _nick(old._nick), _name(old._name), _chanels(old._chanels),
_isRegistered(old._isRegistered), _isAuthenticated(old._isAuthenticated),
_isOperator(old._isOperator), _buf(old._buf){
}

Client &Client::operator=(const Client &old){
	if (this != &old)
	{
		this->_fd = old._fd;
		this->_nick = old._nick;
		this->_name = old._name;
		this->_isRegistered = old._isRegistered;
		this->_isAuthenticated = old._isAuthenticated;
		this->_isOperator = old._isOperator;
		this->_chanels = old._chanels;
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

void Client::setReal(std::string real){
	this->_realname = real;
}

void Client::setHost(std::string host){
	this->_hostname = host;
}

void Client::setBuf(char *buf){
	this->_buf = buf;
}

void Client::setClientCmd(const std::string &cmd)
{
    _cmd = cmd;
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

std::string Client::getClientCmd() const
{
    return (_cmd);
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


void Client::addChanel(const Chanel& ch)
{
    for (std::vector<std::string>::iterator iter = _chanels.begin(); iter != _chanels.end(); ++iter)
    {
        if (*iter == ch.getChanelName())
            return;
    }
    _chanels.push_back(ch.getChanelName());
}

const std::vector<std::string> &Client::getChanels() const
{
    return (_chanels);
}

void Client::deleteChanel(const Chanel& ch)
{
    for(std::vector<std::string>::iterator iter = _chanels.begin(); iter != _chanels.end(); ++iter)
    {
        if((*iter) == ch.getChanelName())
        {
            _chanels.erase(iter);
            return ;
        }
    }
}

//Espero que este sea util al final. Envia el <std::string msg> al fd del cliente.
void Client::MsgToMe(std::string msg)
{
    if (msg.length() > 510)
        msg.erase(510);
    std::cout << _fd << " " << msg << "\n";
    msg += "\r\n";

    ssize_t total = 0;
    ssize_t length = static_cast<ssize_t>(msg.size());
    while (total < length)
    {
        ssize_t n_bytes = send(_fd, msg.c_str() + total, length - total, 0);
        if (n_bytes < 0)
            throw std::runtime_error(strerror(errno));
        total += n_bytes;
    }
}
