#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include<stdlib.h>
#include<sys/wait.h>

void p_c(int signum)
{
	//printf("I received signal from parent\n");
	write(1, "Received sig from parent\n", sizeof "Received sig from parent\n" - 1);
}

void c_p(int signum)
{
	//printf("I received signal from child\n");
	write(1, "Received sig from child\n", sizeof "Received sig from child\n" - 1);
}

int main()
{
	signal(SIGUSR1,p_c);
	signal(SIGUSR2,c_p);

	int c = fork();
	if(c>0)
	{
		printf("Parent says hello to you!\n");
		kill(c,SIGUSR1);  //here c is id of child process
		wait(NULL);
	}
	else
	{
		printf("Chhotu says hii to all!\n");
		kill(getppid(),SIGUSR2);  //signalling to parent
	}

}