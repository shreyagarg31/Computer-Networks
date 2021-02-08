#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
using namespace std;

struct mymsg{
	long type;
	char mdata[512];
};

int main()
{
	struct mymsg mq1;

    key_t key=ftok("ms1",65);
    int msgid=msgget(key,0666 | IPC_CREAT);

    cout<<"PID of process 2: "<<getpid()<<endl;

    mq1.type=2;
    strcpy(mq1.mdata,"Hello from process P2\n");
    msgsnd(msgid,&mq1,sizeof(mq1.mdata),0);
    return 0;

}