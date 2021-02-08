#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<bits/stdc++.h>
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
	sleep(2);
	vector<long> v; 
	while( msgrcv(msgid,&msg1,sizeof(msg1),0,IPC_NOWAIT)!=-1)
	{
       v.push_back(msg1.type);
       cout<<"Received pid is: "<<msg1.type<<endl;
	}
	int pd = getpid();
	for(int i=0;i<v.size();i++)
	{
		msg1.type = v[i];
		sprintf(msg1.mdata,"%d",pd);
		msgsnd(msgid,&msg1,sizeof(msg1),0);
	}
	cout<<"Sent\n";
	//while(1);
	sleep(2);
	msgctl(msgid,IPC_RMID,NULL);
}