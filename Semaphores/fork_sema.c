#include<stdio.h>
#include <semaphore.h> 
#include <unistd.h> 
#include <fcntl.h>
#define name1 "process1"
#define name2 "process2"
void p2() 
{ 
int t=2;
sem_t *s1 = sem_open(name1,O_EXCL);
sem_t *s2 = sem_open(name2,O_EXCL);
sem_wait(s2);
while(t--)
{
    printf("\nP2:S3\n");  
    printf("\nP2:S4\n"); 
    sem_post(s1);
    sem_wait(s2);
 } 
}
void p1() 
{ 
sem_t *s1 = sem_open(name1,O_EXCL);
sem_t *s2 = sem_open(name2,O_EXCL);
int t=2;
while(t--)
{
    printf("\nP1:S1\n"); 
    printf("\nP1:S2\n"); 
    sem_post(s2); 
    sem_wait(s1);
}
sem_post(s2);
} 
int main() 
{ 
    sem_t *s1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);  //read and write permission
    sem_t *s2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);
    pid_t c=fork();
    if(c>0)
    {
        p1();
        printf("Done by parent\n");
    }
    else{
        printf("Hello from child");
        p2();
        printf("Done by child\n");
    }	
    sem_unlink(name1);
    sem_unlink(name2); 
    return 0; 
}