#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include<stdlib.h>

void handle_func(int);

int main()
{
	signal(SIGUSR1,handle_func);
	while(1){
        printf("Executing program\n");
        sleep(1);
	    //raise(SIGINT);
	    //raise(SIGUSR1);
	    kill(getpid(),SIGINT);
	}
}

void handle_func(int signum)
{
	printf("Handling user signal\n");
}