#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int main()
{
	mkfifo("serve",0666);
	mkfifo("C1",0666);
	mkfifo("C2",0666);
	while(1){
	int fd = open("serve",O_RDONLY);
	cout<<"Serve Fifo is opened\n";
	char buff[2][1024],c;
	int j=0,cnt=0;

	while(read(fd,&c,1))
	{
		if(c=='$')
		{
			buff[cnt][j++]='\0';
			j=0;
			cnt++;
		}
		else
		{
			buff[cnt][j++] = c;
		}
	}

	//read(fd,&buff,1024);
	cout<<"Message read from group by server: \n";
	for(int i=0;i<2;i++)
	{
		int k=0;
		while(buff[i][k++]!='\0')
		{
			cout<<buff[i][k];
		}
		cout<<"\n";
	}
	int fd2 = open("C1",O_WRONLY);
	int fd3 = open("C2",O_WRONLY);
	write(fd3,buff[0],1024);
	write(fd2,buff[0],1024);
	write(fd3,buff[1],1024);
	write(fd2,buff[1],1024);
	close(fd2);
	close(fd3);
	close(fd);
   }
	return 0;
}