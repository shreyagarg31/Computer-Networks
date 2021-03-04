#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include<stdlib.h>
#include <string.h> 

int main()
{
	while(1)
	{
		char buf[1024];
		strcpy(buf,"");
		//fflush(stdin);
		//fgets(buf,sizeof(buf),stdin);
		read(0,buf,sizeof(buf));
		buf[strlen(buf)-1] = '\0';
		if(!strcmp(buf,"@"))break;
		printf("%s\n",buf);
		fflush(stdout);
		printf("S1 processed the message!\n");
		fflush(stdout);
	}
	printf("S1 terminated!\n");
	return 0;
}