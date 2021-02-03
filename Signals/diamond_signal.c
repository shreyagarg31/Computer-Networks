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
#include<signal.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include<stdio.h>



char* str2,*str1;
char *a="#";
 int pd,c=0,fd1,fd2,shmid1,shmid2;

void handler_func(int signum){
    
    printf("Writing to shared memory X: \n");
    //write(1,"Writing to X:\n",sizeof"Writing to X:\n"-1);

    strcpy(str1,strcat(str2,a));
    c++;
   // printf("%s\n",str1);
    write(1,str1,1024);
    write(1,"\n",sizeof "\n"-1);
    if(c==10)
    {
        close(fd1);
        close(fd2);

        shmdt(str1); 
        shmdt(str2); 

        shmctl(shmid1,IPC_RMID,NULL);
        shmctl(shmid2,IPC_RMID,NULL); 
        raise(SIGINT); 

    }
    kill(pd,SIGUSR1); //again call
   // wait(NULL);
   // sleep(2);  //wait to catch the signal avoiding termination
}  

int main(){
   
   
     mkfifo("Px",0666);
     mkfifo("Py",0666);
     fd1=open("Px",O_WRONLY);
    fd2=open("Py",O_RDONLY); //created pipes for pid passing

    char buf1[1024],buf2[1024];
    sprintf(buf1,"%d",getpid());
    write(fd1,buf1,1024);  //write this P1 pid to pipe
    read(fd2,buf2,1024);

   
    sscanf(buf2,"%d",&pd); //read pid of P2 from pipe

    key_t key1 = ftok("shmx",65); 
    key_t key2 = ftok("shmy",66); 
  
    shmid1 = shmget(key1,1024,0666|IPC_CREAT);
    shmid2 = shmget(key2,1024,0666|IPC_CREAT);  
  
    str1 = (char*) shmat(shmid1,(void*)0,0); 
    str2 = (char*) shmat(shmid2,(void*)0,0); 
    
    strcpy(str1,"abc");
 
  //at first without waiting it reads value
    signal(SIGUSR2,handler_func);
    while(1){
    kill(pd,SIGUSR1);
    }
    //wait(NULL);
    //sleep(8);

    printf("Going to delete!\n");
   
}