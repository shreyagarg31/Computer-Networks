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
int val=0;
char buff1[1024],buff2[1024];
sigset_t set;
int child_pds[4];
int* kill_val;
int * str1;


char* c1 = "c1";
char* c2 = "c2";
char* sc1="sc1";
char* sc2="sc2";
char* a = "a";
char* b = "b";

void handler(int signum)
{
	if(signum==SIGUSR1)
	{
		printf("Sending signal to Group1\n");
		int kills=*kill_val;
		*kill_val=kills+3;
		killpg(child_pds[3],SIGSTOP);
		killpg(child_pds[0],SIGCONT);
		killpg(child_pds[0],SIGUSR1);

		int fd_sc1=open(sc1,O_WRONLY);
		//write(fd_sc1,"message from S to G1\n",sizeof "message from S to G1\n");
		write(fd_sc1,buff1,sizeof(buff1));

		int fd_sc2=open(sc2,O_WRONLY);
		//write(fd_sc2,"message from S to G1\n",sizeof "message from S to G1\n");
		write(fd_sc2,buff1,sizeof(buff1));
	}
	else
	{
        printf("Sending signal to Group2\n");
		int kills=*kill_val;
		*kill_val=kills+3;
		killpg(child_pds[0],SIGSTOP);
		killpg(child_pds[3],SIGCONT);
		killpg(child_pds[3],SIGUSR1);

		int fd_sc1=open(a,O_WRONLY);
		//write(fd_sc1,"message from S to G2\n",sizeof "message from S to G2\n");
		write(fd_sc1,buff1,sizeof(buff1));

		int fd_sc2=open(b,O_WRONLY);
		//write(fd_sc2,"message from S to G2\n",sizeof "message from S to G2\n");
		write(fd_sc2,buff1,sizeof(buff1));
	}
}

void sc1_handler(int sig)
{
	printf("I S1 got interrupted by a signal from S\n");
	char buff[1024];
	int fd_sc1=open(sc1,O_RDONLY);
	read(fd_sc1,buff,1024);
	close(fd_sc1);

	printf("Message received in S1 :  %s\n",buff);
	int kills=*kill_val;
	*kill_val=kills+1;
	//killpg(child_pds[0],SIGSTOP);
}

void sc2_handler(int sig)
{
	printf("I S2 got interrupted by a signal from S\n");
	char buff[1024];
	int fd_sc2=open(sc2,O_RDONLY);
	read(fd_sc2,buff,1024);
	close(fd_sc2);

	printf("Message received in S2:  %s\n",buff);
}

void a_handler(int sig)
{
	printf("A got interrupted by a signal from S\n");
	char buff[1024];
	int fd_a=open(a,O_RDONLY);
	read(fd_a,buff,1024);
	close(fd_a);

	printf("Message received in A:  %s\n",buff);
	int kills=*kill_val;
	*kill_val=kills+1;
	//killpg(child_pds[3],SIGSTOP);
}

void b_handler(int sig)
{
	printf("B got interrupted by a signal from S\n");
	char buff[1024];
	int fd_b=open(b,O_RDONLY);
	read(fd_b,buff,1024);
	close(fd_b);

	printf("Message received in B:  %s\n",buff);
}

void *polling(void* args)
{
	pthread_sigmask(SIG_BLOCK,&set,NULL);  //prevent signals from blocking the poll

	struct pollfd pfd[2];
	int c1_pid=open(c1,O_RDWR|O_NONBLOCK);
	int c2_pid=open(c2,O_RDWR|O_NONBLOCK);
	pfd[0].fd=c1_pid;
	pfd[1].fd=c2_pid;

	pfd[0].events=POLLIN;
	pfd[1].events=POLLIN;

	while(1)
	{
		int ret=poll(pfd,2,5000);
		if(ret!=0)
		{
			if(ret==-1)
			{	
				printf("Error in polling\n");
				exit(1);
			}
			char buff[1024];
			for(int i=0;i<2;i++)
			{
				if(pfd[i].revents & POLLIN)
				{
					if(read(pfd[i].fd,buff,1024) > 0)
					{

						strcpy(buff1,buff);
                        fflush(fdopen(pfd[i].fd,"r+"));
					}
				}
			}
		}
	}
}

int main()
{
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	/*
	SHARED MEMORY CREATION FOR:
		1) SHARING PID OF S TO D
		2) KILL CALL COUNTS
	*/
	key_t key1 = ftok("sig",65); 
    key_t key2 = ftok("kill",64);
  
    int shmid1 = shmget(key1,sizeof(int),0666|IPC_CREAT); 
    int shmid2 = shmget(key2,sizeof(int),0666|IPC_CREAT);
  
    str1 = (int*) shmat(shmid1,(void*)0,0);
    *str1 = 0;
    kill_val = (int*)shmat(shmid2,(void*)0,0);
    int pd = getpid();
    *str1 = pd;
    printf("Sent id is : %d\n",pd);

    /*FIFO CREATION FOR C1 AND C2*/
	mkfifo(c1,0666|IPC_CREAT);
	mkfifo(c2,0666|IPC_CREAT);

	/*FIFO CREATION FOR THE CHILDREN*/
	mkfifo(sc1,IPC_CREAT|0666);
	mkfifo(sc2,IPC_CREAT|0666);
	mkfifo(a,IPC_CREAT|0666);
	mkfifo(b,IPC_CREAT|0666);

	//CREATE CHILDREN NOW

	int c = fork();
	if(c==0)
	{
       signal(SIGUSR1,sc1_handler);
       printf("First forked child started S1\n");
       while(1)                     //PREVENT CHILD FROM TERMINATING
       {
       	  printf("S1 is running\n");
       	  sleep(2);
       }
	}
	else
	{
	  child_pds[0]=c;	
      int c2 = fork();
      if(c2==0)
      {
	        signal(SIGUSR1,sc2_handler);
	        printf("Second forked child started S2\n");
	        while(1)
	        {
	       	  printf("S2 is running\n");
	       	  sleep(2);
	        }
      }
      else
      {
        child_pds[1] = c2;
        int c3 = fork();
        if(c3==0)
        {
            signal(SIGUSR1,a_handler);
	        printf("Third forked child started A\n");
	        while(1)
	        {
	       	  printf("A is running\n");
	       	  sleep(2);
	        }
        }
        else
        {
          child_pds[2] = c3;
          int c4 = fork();
          if(c4==0)
          {
          	signal(SIGUSR1,b_handler);
	        printf("Fourth forked child started B\n");
	        while(1)
	        {
	       	  printf("B is running\n");
	       	  sleep(2);
	        }
          }
          else
          {
          	child_pds[3] = c4;
          	sleep(2);

          	// create two groups for children
          	setpgid(child_pds[3],child_pds[3]); //B
          	setpgid(child_pds[2],child_pds[3]); //A

          	setpgid(child_pds[0],child_pds[0]); //S`1
          	setpgid(child_pds[1],child_pds[0]); //S`2

          	killpg(child_pds[0],SIGSTOP);
          	killpg(child_pds[3],SIGSTOP);

          	sigemptyset(&set);
			sigaddset(&set,SIGUSR1);
			sigaddset(&set,SIGUSR2);

			pthread_t t1;                             //THREAD TO HANDLE POLLING INDEPENDENTLY
			pthread_create(&t1,NULL,polling,NULL);
			pthread_join(t1,NULL);
          }
        }
      }
	}
}