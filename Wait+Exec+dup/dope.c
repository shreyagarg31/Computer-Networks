#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>

int main()
{
	int fd = open("text2.txt",O_RDONLY);
	int c = dup2(fd,0);
	if(c==-1)
	{
		printf("Error\n");
		exit(0);
	}
	char val;
	for(int i=0;i<3;i++){scanf("%c",&val);printf("%c",val);}
		printf("\n");
	int t = dup2(0,STDIN_FILENO);
    if(t==-1)
    {
    	printf("Error again\n");
    	exit(0);
    }
    scanf("Enter the character:%c",&val);
    printf("%c",val);
}