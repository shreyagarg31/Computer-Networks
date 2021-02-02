#include<stdio.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
  
sem_t s1,s2;
void* p2(void* arg) 
{ 
    sem_wait(&s2);
    for(int i=0;i<2;i++)
    {
        printf("\nP2:S3\n");  
        printf("\nP2:S4\n"); 
        sem_post(&s1);
        sem_wait(&s2);
    } 
}
void* p1(void* arg) 
{ 
    for(int i=0;i<2;i++)
    {
        printf("\nP1:S1\n"); 
        printf("\nP1:S2\n"); 
        sem_post(&s2); 
        sem_wait(&s1); //put on hold in a while loop of wait
    }
    sem_post(&s2); //to break the while loop of wait on s2 done in p2
 } 
int main() 
{ 
     pthread_t t1,t2;
    sem_init(&s1,0,0); 
    sem_init(&s2,0,0);
    pthread_create(&t1,NULL,p1,NULL); 
    pthread_create(&t2,NULL,p2,NULL); 
    pthread_join(t1,NULL); 
    pthread_join(t2,NULL); 
    sem_destroy(&s1);
    sem_destroy(&s2); 
    return 0; 
}