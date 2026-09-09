*This project has been created as part of the 42 curriculum by dgargant, frlorenz and mfornovi*

# Description
IRC is a project that introduces students into the magical world of "creating your own bare bones version of Discord". Basically, we create a internet server to chat through. Just like discord, but *we* have to do it from 0 *and* using C++. Sounds harder than it is tedious. To put it more academically: in IRC we have to create an **I**nternet **R**elay **C**hat server that Clients can connect to, enter the password, register with a nickname and username, and then chat with eachother through either private messages or channels (we know we mispelt it in the code).

# Instructions
First, compile the project using `make`. To run it use `./irc [port] [password]`, for example: `./irc 6667 1234`. Now you can connect to the server by introducing that port and the ip to the computer the server is runninng in (you can use "localhost" if you're connecting from the same computer). Using `nc [ip] 6667`, `telnet [ip] 6667` or Hexchat like so (first you "add" then change the name and then "edit"):

![Hexchat1](Hexchat1.png)
![Hexchat2](Hexchat2.png)

Hexchat introduces the username, nickname and pasword automatically, but if you choose another client you'll have to introduce those manually like so: USER \<nick> 0 * \<user> (if the nickname is in use you'll have to set it with "NICK \<nick>"), PASS \<password>. (the order is irrelevant, but you have to complete registratio before aything else can be done).

Once you're registered and authenticated you can access the rest of the functions:
- JOIN #\<channel name> [\<channel pass>] -> if the channel doesn't exist it creates one and makes you the operator. You can also create several channels at a time.
- PRIVMSG \<recipient/s> :\<message> -> the recipients can be channels.
- KICK \<nickname> \<reason>
- INVITE \<nickname> \<channel> -> all channel names start with "#".
- TOPIC \<channel> [:\<newtopic>] -> if you don't include a new topic it'll show the current topic.
- MODE \<channel> \<mode>:
	- [+/-] i -> set/remove invite-only channel.
	- [+/-] t -> set/remove restrictions of the topic command to channel operators.
	- [+/-] k -> set/remove channel password.
	- [+/-] o [\<nick>] -> give/take operator privilege.
	- [+/-] l -> set/remove user limit to channel.
# Resources
https://modern.ircdocs.horse/
https://beej.us/guide/bgnet/html/split/index.html
https://github.com/Adiazufano/irc/tree/main
https://github.com/caroldaniel/42sp-cursus-ft_irc/tree/main