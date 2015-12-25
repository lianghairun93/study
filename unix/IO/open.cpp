#include <iostream>
#include <fcntl.h>

using namespace std;

int main()
{
	char buf[64]="0";
	int fd1, fd = open("/etc/issue", O_RDONLY);
	if(fd == -1)
		cerr << "open file fail \n";
	if(read(fd, buf,sizeof(buf)) == -1)
		cerr << "read file fail \n";
	cout << buf << endl;
	string issue = buf;
	if( issue.find("CentOS") != string::npos)
	{
		if((fd1 = creat("/home/centos", 755)) < 0)
			cerr << "create faile \n";
		else
		{
			string in = "hello centos.";
			write(fd1, in.c_str(), in.size());
			close(fd1);
		}
	}
	close(fd);
	return 0;
}
