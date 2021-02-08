#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

long pid;
int wid,rid;

const char* name = "C2";

struct mymsg{
	long type;
	char mdata[100];
};


void *writer(void *arg)
{
	while(1)
	{
		struct mymsg msg,buff;
		msg.type = pid;
		char c;
		int j=0;
		read(STDIN_FILENO,&c,1);
		while(c!='\n')
		{
			msg.mdata[j++]=c;
			read(STDIN_FILENO,&c,1);
		}
		msg.mdata[j]='\0';
		buff.mdata[0]='\0';
		buff.type = pid;
		strcat(buff.mdata,name);
		strcat(buff.mdata,":");
		strcat(buff.mdata,msg.mdata);
		strcat(buff.mdata,"\n");
		msgsnd(wid,&buff,sizeof(buff),0);
	}
}

void *reader(void* arg)
{
	while(1)
	{
		struct mymsg msg;
		while(msgrcv(rid,&msg,sizeof(msg),pid,IPC_NOWAIT)!=-1)
		{
			cout<<msg.mdata<<endl;
			flush(cout);
		}
		
	}
}

int main()
{
	pid = getpid();
	cout<<pid<<" : Client started!\n";
	key_t key1 = ftok(".",65);
	key_t key2 = ftok(".",66);

	wid = msgget(key1,0666|IPC_CREAT);
	rid = msgget(key2,0666|IPC_CREAT);

     
    struct mymsg msg1;
    msg1.type = pid;
    msg1.mdata[0] = '|';
    msg1.mdata[1] = '\0';

    if(msgsnd(wid,&msg1,sizeof(msg1),0)==-1)perror("msgsnd");
    //sleep(4);

    pthread_t wth,rth;
    pthread_create(&rth,NULL,reader,NULL);
    pthread_create(&wth,NULL,writer,NULL);

    pthread_join(rth,NULL);
    pthread_join(wth,NULL);

}