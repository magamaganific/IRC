#include "Chanel.hpp"

Chanel::Chanel()
{
}

Chanel::Chanel(std::string name, std::string pass, int admin)
: _chanel_name(name), _topic(""), _chanel_pass(pass), _chanel_mode(""), _limit(0)
{
	addAdmin(admin);
	addMember(admin);
}


Chanel::Chanel(const Chanel &obj)
: _chanel_name(obj._chanel_name),
_topic(obj._topic), _chanel_pass(obj._chanel_pass), _chanel_mode(obj._chanel_mode), _limit(obj._limit), _admins(obj._admins), _members(obj._members),
_guests(obj._guests)
{}

Chanel &Chanel::operator=(const Chanel &obj)
{
	if (this != &obj)
    {
        _chanel_name = obj._chanel_name;
		_chanel_mode = obj._chanel_mode;
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
    return _limit;
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

void Chanel::setLimit(int n)
{
    _limit = n;
}

void Chanel::setMode(char modechar)
{
    if (!isModed(modechar))
        _chanel_mode.append(1, modechar);
}


bool Chanel::isModed(char modechar) const
{
    std::size_t pos = _chanel_mode.find(modechar);
    if (pos != std::string::npos)
        return true;
    return false;
}

bool Chanel::isMember(int fd) const
{
    for(std::vector<int>::const_iterator i = _members.begin(); i != _members.end(); i++)
        if((*i) == fd)
            return true;
    return false;   
}

bool Chanel::isGuest(int fd) const
{
    for(std::vector<int>::const_iterator i = _guests.begin(); i != _guests.end(); i++)
        if((*i) == fd)
            return true;
    return false;   
}

bool Chanel::isAdmin(int fd) const
{
    for(std::vector<int>::const_iterator i = _admins.begin(); i != _admins.end(); i++)
        if((*i) == fd)
            return true;
    return false;
}



void Chanel::unsetMode(char mode_str)
{
    std::size_t pos = _chanel_mode.find(mode_str);
    if (pos != std::string::npos)
    {
        _chanel_mode.erase(pos, 1);
        if (mode_str == 'l')
            _limit = 0;
        else if (mode_str == 'k')
            _chanel_pass.clear();
    }
}


void Chanel::removeFromGuests(int fd)
{
    for (std::vector<int>::iterator i = _guests.begin(); i != _guests.end(); ++i)
    {
        if (*i == fd)
        {
            _guests.erase(i);
            return;
        }
    }    
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


void Chanel::removeFromAdmins(Server &s,int fd)
{
    for (std::vector<int>::iterator i = _admins.begin(); i != _admins.end(); ++i)
    {
        if (*i == fd)
        {
            _admins.erase(i);
            break;
        }
    }
    if (_admins.empty())
    {
        for (size_t i = 0; i < _members.size(); ++i)
        {
            if (_members[i] == fd)
                continue;
            if (s.findClientbyFd(_members[i]))
            {
                addAdmin(_members[i]);
                break;
            }
        }
    }
}


void Chanel::sendMsgToMembers(Server *s, std::string msg, int fd) const
{
    for (size_t i = 0; i < _members.size(); ++i)
    {
        if (s->findClientbyFd(_members[i]) && _members[i] != fd)
        {
            s->SendMsg(_members[i], msg);
        }
    }
}

void Chanel::removeFromAdmins(Server &s,int fd)
{
    for (std::vector<int>::iterator i = _admins.begin(); i != _admins.end(); ++i)
    {
        if (*i == fd)
        {
            _admins.erase(i);
            break;
        }
    }
    if (_admins.empty())
    {
        for (size_t i = 0; i < _members.size(); ++i)
        {
            if (_members[i] == fd)
                continue;
            if (s.findClientbyFd(_members[i]))
            {
                Client &op = s.getClientbyFd(_members[i]);
                addAdmin(_members[i]);
                sendMsgToMembers(&s, CMD_MODE(_chanel_name, "+o", op.getNick()));
                break;
            }
        }
    }
}

void Chanel::removeClient(Server &s, int fd)
{
    if(isAdmin(fd))
        removeFromAdmins(s, fd);
    for (std::vector<int>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it == fd)
        {
            _members.erase(it);
            return;
        }
    }
}

Chanel::~Chanel()
{}
