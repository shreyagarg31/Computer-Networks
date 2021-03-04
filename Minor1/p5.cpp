#include<stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <iostream>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>

#define name "www"
using namespace std;

int main(){
	char buf[1024];
	sem_t *s1 = sem_open(name,O_CREAT,0666,0);
	int value;
    sem_getvalue(s1,&value);
    cout<<value<<endl;
	int infd = fileno(popen("./p1","r"));
	dup2(infd,0);
	int t=15;
	while(t--){
	fgets(buf,sizeof(buf),stdin);
	cout<<buf<<endl;
  }
   sem_post(s1);
	return 0;
}