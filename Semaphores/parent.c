#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<semaphore.h>
#define name1 "parent"
#define name2 "child"

char g;

void p1(int fd)
{
	sem_t *s1 = sem_open(name1,O_EXCL);
    sem_t *s2 = sem_open(name2,O_EXCL);


    while(read(fd,&g,1)==1)
    { 
      //printf("$");
      if(g=='\n')
      {
      	printf("\n");
      	sem_post(s2);
      	sem_wait(s1);
      }
      else if(g==' ')printf(" ");
      else
      {
      	printf("%c",g);
      }
    }

    sem_post(s2);
}

void p2(int fd)
{
  sem_t *s1 = sem_open(name1,O_EXCL);
  sem_t *s2 = sem_open(name2,O_EXCL);
  sem_wait(s2);

  while(read(fd,&g,1)==1)
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
	sem_t *s1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);  //read and write permission
    sem_t *s2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);
    int fd = open("text.txt", O_RDONLY);
    pid_t c = fork();
    if(c>0)
    {
    	p1(fd);
    }
    else 
    	{  
    		p2(fd);
    	}
    sem_unlink(name1);
    sem_unlink(name2); 
    close(fd);
    return 0;
}