#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<semaphore.h>
#define name1 "parent"
#define name2 "child"


void p2()
{
  sem_t *s1 = sem_open(name1,O_EXCL);
  sem_t *s2 = sem_open(name2,O_EXCL);
  //printf("In p2()\n");
  sem_wait(s2);
  char g;

 while(read(0,&g,1)==1)
  {
     if(g=='\n')
      {
      	printf("\n");
      	sem_post(s1);
      	sem_wait(s2);
      }
      else if(g==' ')printf(" ");
      else
      {
      	printf("%c",g);
      }
  }
  sem_post(s1);
}


int main()
{
	printf("We are in process 2 now\n");
	p2();
}