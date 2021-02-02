#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	printf("From P1: Hello Guys!!!\n");
	FILE* fd=popen("./q72","r");
	char buf[1024];
	read(fileno(fd),buf,1024);
	printf("From pipe: %s",buf);
	pclose(fd);
	return 0;
}