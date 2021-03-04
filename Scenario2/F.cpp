#include<unistd.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>
#include <sys/msg.h> 
#include<bits/stdc++.h>

#define name "F1"
#define name1 "S1"
#define name2 "S2"
#define name3 "S3"

using namespace std;

int main()
{
	sem_t *s = sem_open(name,O_CREAT,0666,0);
	if(s==SEM_FAILED)perror("s");
	int i=3;
	while(i--)sem_wait(s);
	sem_t *s1 = sem_open(name1,O_EXCL);
	if(s1==SEM_FAILED)perror("s1");
	sem_t *s2 = sem_open(name2,O_EXCL);
	if(s2==SEM_FAILED)perror("s2");
	sem_t *s3 = sem_open(name3,O_EXCL);
	if(s3==SEM_FAILED)perror("s3");

	
    sem_post(s1);
    sem_post(s2);
    sem_post(s3);

    cout<<"Signalled to all processes\n";

    sem_unlink(name);
    sem_unlink(name1);
    sem_unlink(name2);
    sem_unlink(name3);
}
