#include "poll.h"

CPoll::CPoll()
{
	for(int i = 0; i < 4096; i++)
	{
		clientfd[i].fd = -1;
	}
	m_epfd = 0;
}
CPoll::~CPoll()
{
}

void CPoll::BindEvt(int skfd)
{
	for(int i = 0; i< 4096; i++)
	{
		if(clientfd[i].fd == -1)
		{
			clientfd[i].events = POLLIN|POLLOUT|POLLPRI;
			clientfd[i].fd = skfd;
			break;
		}
	}
}

void CPoll::EpollMain(int listenfd)
{
//	epoll_event evt[EPMAX];
	int fd;
	while(1)
	{
		//int n =epoll_wait(m_epfd, evt, EPMAX, 0);
		int n = poll(clientfd, m_epfd +1,0);
		for(int i = 0; i < n; ++i)
		{
			fd = clientfd[i].fd;	
	//		if(evt[i].events & EPOLLIN)
	//			Read(fd);
#if 1
			if(clientfd[i].revents & POLLIN)
			{
				struct sockaddr_in cliaddr;
				socklen_t cliaddrlen;
				int clifd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
				if(clifd == -1)
					cerr << "accept error." << endl;
				else
				{
					printf("recv form port:%d,clifd=%d,i=%d\n",cliaddr.sin_port,clifd,i);
					BindEvt(clifd);
					break;
				}
			}
	//		else if(clientfd[i].revents & POLLIN)
	//		{
	//			printf("recv form clifd=%d\n",fd);
	//			Read(fd);
	//		}
			m_epfd = (i > m_epfd ? i : m_epfd);
#endif
		}
		for(int i = 1; i <= m_epfd; ++i)
		{
			if(clientfd[i].fd < 0)
				continue;
			if(clientfd[i].revents & POLLIN)
				Read(clientfd[i].fd);
		}
	}
}

int CPoll::Read(int fd)
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
