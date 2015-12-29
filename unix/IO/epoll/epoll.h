#ifndef __I2_EPOLL_H__
#define __I2_EPOLL_H__

#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>


#define EPMAX (64)

using namespace std;

class CEpoll
{
public:
	CEpoll();
	virtual ~CEpoll();
	void BindEvt(int skfd);
	void EpollMain(int listenfd);
private:
	int Read(int fd);
	int m_epfd;
};



#endif
