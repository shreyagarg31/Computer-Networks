#include<unistd.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<errno.h>

#define name1 "processx"
#define name2 "processy"

int main()
{
	key_t key = ftok("shmx",65);
	key_t key2 = ftok("shmy" , 66);
	sem_t *s1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);  //read and write permission
    sem_t *s2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);
    if(s1 == SEM_FAILED || s2 == SEM_FAILED)
	{
		   printf("Error occured!\n");  
		   perror("Error\n");  
        	sem_unlink(name1);
        	sem_unlink(name2);
        	exit(1);
	}
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    int *x = (int*)shmat(shmid,(void*)0,0);
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT);
    int *y = (int*)shmat(shmid2,(void*)0,0);
    *x = 0;
    *y = 1;
    int t=3;
    while(t--)
    {
    	printf("P1 read value of y:%d\n",*y);
        *x = *y + 3;
        printf("Value written by P1!: %d\n",*x);
        sem_post(s1);
        printf("P1 is waiting for P2 to write \n");
        sem_wait(s2);
    }


    sem_unlink(name1);
    sem_unlink(name2); 
    shmdt(x);
    shmctl(shmid,IPC_RMID,NULL); 
    shmdt(y);
    shmctl(shmid2,IPC_RMID,NULL); 
    return 0;

}