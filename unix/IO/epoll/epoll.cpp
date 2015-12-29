#include "epoll.h"

CEpoll::CEpoll()
{
	m_epfd = epoll_create(EPMAX);
	if(m_epfd < 0)
		fprintf(stderr,"epoll_create %s.\n",strerror(errno));
}
CEpoll::~CEpoll()
{
	close(m_epfd);
}

void CEpoll::BindEvt(int skfd)
{
	epoll_event e;
	e.events = EPOLLIN|EPOLLET|EPOLLOUT|EPOLLPRI|EPOLLRDHUP;
	e.data.fd = skfd;
	if(epoll_ctl(m_epfd, EPOLL_CTL_ADD, skfd, &e) < 0)
		fprintf(stderr, "epoll_ctl %s\n",strerror(errno));
}

void CEpoll::EpollMain(int listenfd)
{
	epoll_event evt[EPMAX];
	int fd;
	while(1)
	{
		int n =epoll_wait(m_epfd, evt, EPMAX, 0);
		for(int i = 0; i < n; ++i)
		{
			fd = evt[i].data.fd;	
	//		if(evt[i].events & EPOLLIN)
	//			Read(fd);
#if 1
			if((fd == listenfd) && (evt[i].events & EPOLLIN))
			{
				struct sockaddr_in cliaddr;
				socklen_t cliaddrlen;
				int clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
				if(clifd == -1)
					cerr << "accept error." << endl;
				else
				{
					printf("recv form port:%d,clifd=%d\n",cliaddr.sin_port,clifd);
					BindEvt(clifd);
				}
			}
			else if(evt[i].events & EPOLLIN)
			{
				printf("recv form clifd=%d\n",fd);
				Read(fd);
			}
#endif
		}
	}
}

int CEpoll::Read(int fd)
{
	char buf[512];
	int n = read(fd, buf, 512);
	if(n == -1)
		cerr << "read error." << endl;
	else if(n == 0)
		cerr << "client close." << endl;
	else
		cout << "read message is :" << buf << endl;
	return 0;
}
