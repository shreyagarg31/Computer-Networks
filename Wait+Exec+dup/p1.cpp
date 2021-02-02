#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;
int main()
{
	cout<<"Hello from Process1"<<"\n";
	execvp("./p2",NULL);
	cout<<"Ending"<<"\n";

}