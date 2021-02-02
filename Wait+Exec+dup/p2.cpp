#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;
int main()
{
	cout<<"Hello from Process2 called by Process1"<<"\n";
	execvp("./p3",NULL);
	return 0;

}