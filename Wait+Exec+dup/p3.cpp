#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;
int main()
{
	cout<<"Hello from Process3 called by Process2"<<"\n";
	execvp("./p4",NULL);
	return 0;

}