#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define MAX 5


int main()
{
	pid_t c=fork();
	if((int)c>0)
	{
		printf("Process: %d started\n",(int)getpid());
		sleep(3);
		kill((int)c,SIGINT);
		//wait(NULL);
	}
	else
	{
		//printf("child\n");
		char *args[]={"./siginfo2",NULL}; 
        execvp(args[0],args);
	}
	return 0;
}