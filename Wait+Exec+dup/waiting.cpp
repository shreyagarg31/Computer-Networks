#include<iostream>
#include<sys/wait.h>
#include<unistd.h>

using namespace std;
int main(){

	cout<<"Parent going to create child process p2 and p3"<<"\n";
	pid_t p2 = fork();
	pid_t p3 = fork();

	if(p2>0 && p3>0)
	{
		wait(NULL);
		wait(NULL);
		cout<<"Parent terminating"<<"\n";
	}
	else if(p2==0 && p3>0)
	{
		wait(NULL);
		cout<<"Process p2 termination"<<"\n";
	}
	else if(p2>0 && p3==0)
	{
		cout<<"Process p3 termination"<<"\n";
	}
	else cout<<"Process p4 termination(grand child)"<<"\n";
}