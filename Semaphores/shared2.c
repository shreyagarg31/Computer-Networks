#include<unistd.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/shm.h>

#define name1 "processx"
#define name2 "processy"

int main()
{
	key_t key2 = ftok("shmy",66);
    key_t key = ftok("shmx" , 65);
    sem_t *s1 = sem_open(name1,O_EXCL);  //read and write permission
    sem_t *s2 = sem_open(name2,O_EXCL);
     if(s1 == SEM_FAILED || s2 == SEM_FAILED)
    {
           printf("Error occured!\n");    
            sem_unlink(name1);
            sem_unlink(name2);
            exit(1);
    }
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    int* x = (int*)shmat(shmid,(void*)0,0);
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT);
    int* y = (int*)shmat(shmid2,(void*)0,0);
    int t=3;
    while(t--)
    {
        sem_wait(s1);

        printf("Value read by P2 is: %d",*x);
        *y = *x + 3;
        printf("Value written by P2 and waiting for P1 now to write!\n");
        sem_post(s2);

    }
    sem_unlink(name1);
    sem_unlink(name2); 
    shmdt(x);
    shmctl(shmid,IPC_RMID,NULL); 
    shmdt(y);
    shmctl(shmid2,IPC_RMID,NULL); 
    return 0;
}