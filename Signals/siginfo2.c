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

struct sigaction sigac;
void handler(int sig,siginfo_t *siginfo,void* context)
{
	pid_t sender_pid=siginfo->si_pid;
	printf("INTERRUPT received from Process: %d\n",(int)sender_pid);
}

int main()
{
	
	sigac.sa_sigaction = &handler;
	sigac.sa_flags |= SA_SIGINFO; //to get detail information
	
	int k=sigaction(SIGINT,&sigac,NULL);
	if(k!=0)
	{
		printf("Error: Interrupt\n");
		return errno;
	}
	else
	{
		printf("Signal received\n");	
	}
	sleep(5);
	return 0;
}