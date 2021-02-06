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

#define FILLED 0 
#define Ready 1 
#define NotReady -1 

#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */

char* c2 = "c2";

int main()
{
	mkfifo(c2,0666);
	while(1)
    {
        char buff[1024];
        printf("Enter input to send the groups: \n");
        fflush(stdout);
        fgets(buff,1024,stdin);
        
        int fd2 = open(c2,O_RDWR);
        write(fd2,buff,sizeof(buff));
        fflush(fdopen(fd2,"r+"));
        
        close(fd2);
    }
	return 0;
}

