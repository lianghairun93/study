#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 80
#define SERV_PORT 8484

int main()
{
	struct sockaddr_in seraddr;
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
	
	sockfd =socket(AF_INET, SOCK_STREAM, 0);
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr);
	seraddr.sin_port = htons(SERV_PORT);

	connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	printf("start send to:\n");
	write(sockfd, "lianghairun", 32);
	printf("end send to:\n");

	n = read(sockfd, buf, MAXLINE);
	printf("Response from server:\n");
	write(STDOUT_FILENO, buf, n);
	close(sockfd);
	return 0;
}
