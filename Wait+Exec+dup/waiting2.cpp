#include<iostream>
#include<sys/wait.h>
#include<unistd.h>

using namespace std;
int main(){

	cout<<"Parent going to create child process p2"<<"\n";
	pid_t p2 = fork();

	if(p2>0)
	{
		wait(NULL);
		cout<<"Process p2 terminating"<<"\n";
		cout<<"Parent creating process p3"<<"\n";
		pid_t p3 = fork();
		if(p3>0)
		{
			wait(NULL);
			cout<<"Process p3 terminating"<<"\n";
			cout<<"Parent terminating"<<"\n";
		}
		else
		{
			cout<<"Hello from process p3"<<"\n";
			exit(0);
		}

	}
	else 
	{
		cout<<"Hello from process p2"<<"\n";
		exit(0);
	}
}