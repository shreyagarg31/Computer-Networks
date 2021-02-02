#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	FILE* fd=popen("./ex2","w");
	printf("From P1: Hello World!!!\n");
	char buf1[1024];
	scanf("%s",buf1);
	write(fileno(fd),buf1,1024);
	//char buf[1024];
	//read(fileno(fd),buf,1024);
	//printf("From pipe: %s",buf);
	pclose(fd);
	return 0;
}