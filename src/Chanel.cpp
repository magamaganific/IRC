#include "Chanel.hpp"

Chanel::Chanel()
{
}

Chanel::Chanel(std::string name, std::string pass, int admin)
: _chanel_name(name), _chanel_pass(pass), _topic(""), _limit(0)
{
	addAdmin(admin);
	addMember(admin);
}


Chanel::Chanel(const Chanel &obj)
: _chanel_name(obj._chanel_name), _chanel_pass(obj._chanel_pass),
_topic(obj._topic), _limit(obj._limit), _admins(obj._admins), _members(obj._members),
_guests(obj._guests)
{}

Chanel &Chanel::operator=(const Chanel &obj)
{
	if (this != &obj)
    {
        _chanel_name = obj._chanel_name;
        _topic = obj._topic;
        _chanel_pass = obj._chanel_pass;
        _limit = obj._limit;
        _admins = obj._admins;
        _members = obj._members;
        _guests = obj._guests;
    }
    return (*this);
}

const std::string &Chanel::getChanelName() const
{
    return (_chanel_name);
}

const std::string &Chanel::getChanelTopic() const
{
    return (_topic);
}

const std::string &Chanel::getChanelPass() const
{
	return (_chanel_pass);
}

const int &Chanel::getLimit() const
{
    return (this->_limit);
}


const std::vector<int> &Chanel::getChanelAdmins() const
{
    return (_admins);
}

const std::vector<int> &Chanel::getChanelMembers() const
{
    return (_members);
}


void Chanel::setChanelName(const std::string name)
{
    _chanel_name = name;
}


void Chanel::setChanelTopic(const std::string topic)
{
    _topic = topic;
}

void Chanel::setChanelPass(const std::string pass)
{
	_chanel_pass = pass;
}

void Chanel::addMember(int fd)
{
    for (size_t i = 0; i < _members.size(); i++)
        if (_members[i] == fd)
            return;
    _members.push_back(fd);
}

void Chanel::addAdmin(int fd)
{
    for(size_t i = 0; i < _admins.size(); i++)
        if(_admins[i] == fd)
            return;
    _admins.push_back(fd);
}

void Chanel::addGuest(int fd)
{
    for(size_t i = 0; i < _guests.size(); i++)
        if(_guests[i] == fd)
            return;
    _guests.push_back(fd);
}

Chanel::~Chanel()
{
	
}
