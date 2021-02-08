#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

int rid,wid;


struct mymsg{
	long type;
	char mdata[100];
};

void func(int signum)
{
   msgctl(rid,IPC_RMID,NULL);
   msgctl(wid,IPC_RMID,NULL);
   signal(SIGINT,SIG_DFL);
   raise(SIGINT);
}

int main()
{
    key_t key1 = ftok(".",65);
    key_t key2 = ftok(".",66);

	rid = msgget(key1,0666|IPC_CREAT);
	wid = msgget(key2,0666|IPC_CREAT);

	signal(SIGINT,func);

	cout<<"Server started!\n";
	vector<long> v;
	while(1)
	{
		struct mymsg msg;
		if(msgrcv(rid,&msg,sizeof(msg),0,0)==-1)perror("msgrcv");

		if(strcmp(msg.mdata,"|")==0)
		{
			v.push_back(msg.type);
			cout<<msg.type<<" Client is added to network!\n";
		}
		else
		{
			long val=msg.type;
			for(int i=0;i<v.size();i++)
			{
				if(v[i]==val)continue;
				msg.type = v[i];        //send to all other clients in chatroom
				msgsnd(wid,&msg,sizeof(msg),0);
			}
		}
	}
	return 0;
}