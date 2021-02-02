#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	//write(1,"From P2: Hello World!!\n",1024);
	//printf("Hey guys by EX2\n");
	char buf[1024];
	scanf("%s",buf);
	//read(0,buf,1024);
    printf("Here in ex2: %s\n",buf);
	return 0;
}