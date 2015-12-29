#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>

struct pollfd clients[4096];
int idex = 0;

void Read(struct pollfd *clients, int max);
int socket_bind(const char * ip, int port)
{
	int listenfd;
	struct sockaddr_in servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		perror("socket error.");
		exit (1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind error.");
		exit (1);
	}
	return listenfd;
}
void BindEvt(int listenfd)
{
	extern int idex;
	int i=0;
	for(i=0;i<4096;i++)
	{
		if(clients[i].fd == -1)
		{
			idex++;
			clients[i].fd = listenfd;
			clients[i].events = POLLIN;
			break;
		}
	}
}
void init()
{
	int i=0;
	for(i = 0; i < 4096; ++i)
		clients[i].fd = -1;
}
void do_poll(int listenfd)
{
	extern int idex;
	int i,j,max = 0;
	while(1)
	{
		int np = poll(clients, idex, -1);
		printf("index=%d.\n",idex);
		for(i = 0; i < idex; i++)
		{
			int readfds = np;
			if(!(clients[i].revents * POLLIN))
				continue;
			if( clients[i].fd ==listenfd )
			{
				struct sockaddr_in cliaddr;
				socklen_t cliaddrlen = sizeof(cliaddr);
				int connfd;
				if((connfd = accept(listenfd,(struct sockaddr *)&cliaddr, &cliaddrlen)) == -1)
				{
					if(errno == EINTR)
						continue;
					else
						perror("accept error.");
				}
				BindEvt(connfd);
				printf("recv port = %d,np=%d\n",cliaddr.sin_port,np);
			}
			else
			{
				char buf[64];
				read(clients[i].fd, buf, 64);
				printf("recv from:%s\n",buf);
			//	idex--;
			//	clients[i].fd = -1;
			}

		}
	}

}

int main()
{
	int listenfd;
	listenfd = socket_bind("127.0.0.1", 8484);
	listen(listenfd, 5);
	init();
	BindEvt(listenfd);
	do_poll(listenfd);
	return 0;
}
