#include "epoll.h"

int socket_bind(const char *ip, int port);
int main()
{
	int listenfd;
	CEpoll ep;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	listenfd = socket_bind("127.0.0.1",8484);
	listen(listenfd, 5);
#if 0
	int clifd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
	if(clifd == -1)
		cerr << "accept error" << endl;
	else
		ep.BindEvt(clifd);
#endif
	ep.BindEvt(listenfd);
	ep.EpollMain(listenfd);
}
int socket_bind(const char *ip, int port)
{
	int listenfd;
	struct sockaddr_in servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		cerr << "socket error" << endl;
		exit (1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		cerr << "bind error." << endl;
		exit (1);
	}
	return listenfd;
}
