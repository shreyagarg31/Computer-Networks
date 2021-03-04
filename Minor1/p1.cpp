#include<stdio.h>
#include <unistd.h>
#include<poll.h>
#include<iostream>
#include<sys/wait.h>
#include <stdlib.h>
#include<string.h>
using namespace std;

struct pollfd fds[3];
int main()
{
	int fd[3];
	fd[0]=fileno(popen("./p2","r"));
	fd[1]=fileno(popen("./p3","r"));
	fd[2]=fileno(popen("./p4","r"));

	for(int i=0;i<3;++i){
		fds[i].fd = fd[i];
		fds[i].events = POLLIN;
	}

	while(1)
	{ 
		int val=poll(fds,3,-1);
		if (val<=0) {
			printf ("%d seconds elapsed.\n", 0);
			return 0;
		}
		int flag=0;
		for(int j=0;j<3;j++)
		{
			sleep(2);
			cout<<"Here"<<endl;
			if(fds[j].revents & POLLIN)
			{
				char buf[1024];
				fds[j].revents=0; 
				if(read(fds[j].fd,buf,1024))
				{
					flag=1;
					cout<<buf;
				}
			}
		}
		if(flag==0)
		{
			cout<<"I am from messenger from P1\n";
		}
	}
}