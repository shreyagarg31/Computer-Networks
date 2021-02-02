#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
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

int main()
{
	sem_t *s1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);  //read and write permission
    sem_t *s2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);

    int fd = open("text.txt", O_RDONLY);
    dup2(fd,0);
    pid_t c = fork();
    if(c>0)
    {
       p1(fd);
    }
    else
    {
    	execvp("./dup_process2",NULL);
    }
}