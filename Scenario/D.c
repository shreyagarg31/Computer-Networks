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
int main()
{
    key_t key1 = ftok("sig",65); 
    key_t key2 = ftok("kill",64);
  
    int shmid1 = shmget(key1,sizeof(int),0666|IPC_CREAT); 
    if (shmid1 < 0)
    {
        printf(" SHM ERROR SERVER\n");
        return 0;
    }
    int shmid2 = shmget(key2,sizeof(int),0666|IPC_CREAT);
     if (shmid2 < 0)
    {
        printf("COUNT KILL CALL SHM ERROR SERVER\n");
        return 0;
    }
  
    int* str1 = (int*) shmat(shmid1,(void*)0,0);
    int* kill_val = (int*)shmat(shmid2,(void*)0,0);
    int pid = *str1;
    *kill_val = 0;
    printf("Received pid from s: %d \n",pid);
    int  x;
    while(1)
    {
        scanf("%d",&x);
        //printf("Input taken\n");
        if(x==1)
        {
            int count = *kill_val;
            *kill_val = count+1;
            kill(pid,SIGUSR1);
            printf("Signal sent!\n");
        }
        else if(x==2)
        {
            int count = *kill_val;
            *kill_val = count+1;
            kill(pid,SIGUSR2);
        }
        else
        {
            int count = *kill_val;
            printf("The total no of kill calls done are: %d\n",count);
        }

    } 
    return 0;
}