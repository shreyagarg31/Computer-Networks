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
    sem_t *s1 = sem_open(name,O_EXCL);
    int value;
    sem_getvalue(s1,&value);
    cout<<value<<endl;
	sem_wait(s1);
	int infd = fileno(popen("./p1","r"));
	dup2(infd,0);
	while(1){
	fgets(buf,sizeof(buf),stdin);
	cout<<buf<<endl;
  }
     sem_close(s1);
    //sem_unlink(name);
	return 0;
}