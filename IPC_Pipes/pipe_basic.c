#include <stdio.h> 
#include <unistd.h> 
#include<stdlib.h>

int main()
{
	int a[2];
	char buff1[10],buff2[10];
	int r = pipe(a);
	if(r<0)exit(1);
	for(int i=0;i<5;i++)scanf("%c",&buff1[i]);
	write(a[1],buff1,5);
    read(a[0],buff2,5);
    for(int i=0;i<5;i++)printf("%c",buff2[i]);
    printf("\n");
}