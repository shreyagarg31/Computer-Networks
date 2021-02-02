#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<string>
#include<pthread.h>

#define MSG_SZ 500

using namespace std;

void* read_func(void* args)
{
	int* fd_a=(int*)args;
	int fd=*fd_a;
	char buff[MSG_SZ];
	read(fd,buff,MSG_SZ);
	cout<<"Returned value: "<<buff<<"\n";
}

void* write_func(void* args)
{
	int* fd_a=(int*)args;
	int fd=*fd_a;
	char buff[MSG_SZ];
	cout<<"Enter your message";
	cout<<fd<<" :";
	fgets(buff,128,stdin);
	write(fd,buff,MSG_SZ);
}
int main()
{
	int a[2],b[2];
	pipe(a);
	pipe(b);
	pthread_t read_th,write_th;
	pid_t pd = fork();
	if(pd>0)
	{
		close(a[0]);
		close(b[1]);
		while(1)
		{
			pthread_create(&write_th,NULL,&write_func,(void*)&a[1]);
			//sleep(10);
			pthread_create(&read_th,NULL,&read_func,(void*)&b[0]);
			pthread_join(write_th,NULL);	
			pthread_join(read_th,NULL);
		}
	}
	else
	{
		close(a[1]);
		close(b[0]);
		while(1)
		{
			//sleep(4);
			pthread_create(&read_th,NULL,&read_func,(void*)&a[0]);
			sleep(5);
			pthread_create(&write_th,NULL,&write_func,(void*)&b[1]);
			pthread_join(read_th,NULL);
			
			pthread_join(write_th,NULL);
		}
	}
	return 0;
}