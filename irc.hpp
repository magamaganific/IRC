#include <sys/socket.h>
/*[socket()], close(), setsockopt(), getsockname()
	, [bind()], connect(), [listen()], [accept()]
	, [send()], [recv()]*/
#include <netdb.h>
/*getprotobyname(), gethostbyname(), getaddrinfo()
	, freeaddrinfo()*/
#include <arpa/inet.h>
/*[htons()], htonl(), ntohs(), ntohl(), inet_addr()
 , inet_ntop(),*/
#include <netinet/in.h>
/*inet_ntoa()*/
#include <signal.h>
/*signal(), sigaction(), sigemptyset(), sigfillset()
	, sigaddset(), sigdelset(), sigismember()*/
#include <unistd.h>
/*lseek()*/
#include <sys/stat.h>
/*fstat()*/
#include <fcntl.h>
/*fcntl()*/
#include <poll.h>
/*poll()*/
