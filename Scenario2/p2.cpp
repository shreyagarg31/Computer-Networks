#include<unistd.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>
#include <sys/msg.h> 
#include<bits/stdc++.h>

#define name "S2"
#define name2 "F1"

using namespace std;

struct mymsg{
	long type;
	char mdata[100];
};
int msgid;
vector<long> v;

void *writer(void *arg)
{
    while(1)
    {
        struct mymsg msg,buff;
        msg.type = getpid();
        char c;
        int j=0;
        read(STDIN_FILENO,&c,1);
        while(c!='\n')
        {
            msg.mdata[j++]=c;
            read(STDIN_FILENO,&c,1);
        }
        msg.mdata[j]='\0';
        for(int i=0;i<v.size();i++){
            msg.type = v[i];
            cout<<v[i]<<endl;
            int p =msgsnd(msgid,&msg,sizeof(msg.mdata),0);
            if(p==-1)perror("error\n");
      }
    }
}

void *reader(void* arg)
{
    while(1)
    {
        struct mymsg msg;
        int pid = getpid();
        while(msgrcv(msgid,&msg,sizeof(msg.mdata),pid,IPC_NOWAIT)!=-1)
        {
            cout<<msg.mdata<<endl;
            flush(cout);
        }
        
    }
}


int main()
{
    //msgqueue creation

    key_t key1 = ftok(".",10);
     msgid = msgget(key1,0666|IPC_CREAT);

    vector<long> v;
    struct mymsg msg1,msg2;
    int pid = getpid();
    cout<<"PID of P2 is : "<<pid<<endl;
    msg1.type = pid;
    msgsnd(msgid,&msg1,sizeof(msg1.mdata),0);
    msgsnd(msgid,&msg1,sizeof(msg1.mdata),0);
    int cnt = 0;
    while(1)
    {
        msgrcv(msgid,&msg2,sizeof(msg2.mdata),0,IPC_NOWAIT);
    	long val = msg2.type;
    	if(msg2.type==(long)pid)msgsnd(msgid,&msg1,sizeof(msg1.mdata),0);
    	else
    	{
    		if(find(v.begin(),v.end(),val)==v.end())
    		{
    			v.push_back(val);
    			cnt++;
    			if(cnt==2)break;
    			//else msgrcv(msgid,&msg2,sizeof(msg2.mdata),0,0);
    		}
    		else msgsnd(msgid,&msg1,sizeof(msg1.mdata),0);
    	}
    }
    for(int i=0;i<v.size();i++)cout<<v[i]<<endl;

    sem_t *s1 = sem_open(name,O_CREAT,0666,0);
    sem_t *s = sem_open(name2,O_CREAT);

    sem_post(s);
    sem_wait(s1);

     pthread_t wth,rth;
    pthread_create(&wth,NULL,writer,NULL);
    pthread_create(&rth,NULL,reader,NULL);

    pthread_join(wth,NULL);
    pthread_join(rth,NULL);
        //sleep(2);

    sem_unlink(name);
    sem_unlink(name2);
}