 #include <stdio.h>
 #include <unistd.h>
 #include <signal.h>
#include<stdlib.h>
//int cnt=1;
 int a[2];
 
 void handler_function(int);
 
 int main()
 {
     /*
      * attach handler for SIGINT
      */
    //int a[2];
    pipe(a);
   int count=1;

    signal(SIGINT,handler_function);
     while(1)
     {
         printf("Program executing (1s delay between updates)...\n");
         write(a[1],&count,sizeof(count));
         count++;
         sleep(1);
     }
 
     return(0);
 }
 
 
 void handler_function(int signum)
 {
     int cnt;
     read(a[0],&cnt,sizeof(cnt));
     if(cnt>3) signal(SIGINT,SIG_DFL);
     else printf(">>>  executing handler for %d time <<<\n",cnt);
    // signal(SIGINT,SIG_DFL);
 }