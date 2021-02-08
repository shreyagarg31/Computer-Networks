#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

struct mymsg{
	long type;
	char mdata[100];
};

int main()
{
	key_t key1 = ftok("comm",64);
	int msgid = msgget(key1,0666|IPC_CREAT);
	struct mymsg msg1,msg2;
	int pd = getpid();
	msg1.type = pd;
	msgsnd(msgid,&msg1,sizeof(msg1),0);
	cout<<"Sent id from p1 : "<<pd<<endl;
	sleep(5);
	msgrcv(msgid,&msg2,sizeof(msg2),pd,0);
	//cout<<val<<endl;
	cout<<"Received pid from main process: "<<msg2.mdata<<endl;
	sleep(2);
	//while(1);
	msgctl(msgid,IPC_RMID,NULL);
}