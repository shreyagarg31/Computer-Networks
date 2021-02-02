#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<stdlib.h>
int main()
{
    int a[2], b[2];
    pipe(a);
    pipe(b);

    int temp = dup(STDIN_FILENO);
    int temp2 = dup(STDOUT_FILENO);
    dup2(a[0],0);
    dup2(b[1],1);
    int c=fork();
   
    if(c>0)
    {
        dup2(temp,STDIN_FILENO);
        dup2(temp2,STDOUT_FILENO);
        close(a[0]); 
        close(b[1]);
        char buf1[128];
        printf("Writing to P2:");
        scanf("%s",buf1);
        write(a[1],buf1,128);
        char buf2[128];
        read(b[0],buf2,128);
        printf("Printing on screen by parent: %s\n",buf2); //got from other program
        close(a[1]);
        close(b[0]);
    }
    else
    { 
        close(a[1]);
        close(b[0]);
        execv("./pipe42",NULL); 
    }
    
}