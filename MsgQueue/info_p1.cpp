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

int main(){

	struct mymsg mq1;

    key_t key=ftok("ms1",65);
    int msgid=msgget(key,0666 | IPC_CREAT);
     if(msgid<0){
        perror("error");
    }
    cout<<"Process ID: "<<getpid()<<endl;
    struct msqid_ds buf;
    msgctl(msgid, IPC_STAT,&buf);

    cout<<"Before Message receival, no of messages :"<<buf.msg_qnum<<endl;
    cout<<"PID of process who last sent the message :"<<buf.msg_lspid<<endl;
    cout<<"Time of last sent message :"<<buf.msg_stime<<endl;
    msgrcv(msgid,&mq1,sizeof(mq1.mdata),2,IPC_NOWAIT);

    msgctl(msgid, IPC_STAT,&buf);
    cout<<"Time of last received message: "<<buf.msg_rtime<<endl;
    cout<<"After receiveing message, no of messages :"<<buf.msg_qnum<<endl;
    cout<<"PID of process who last received the message :"<<buf.msg_lrpid<<endl;
    cout<<"Access permissions on queue: \n";
    cout<<"Effective UID of owner: "<<buf.msg_perm.uid<<endl;
    cout<<"Effective GID of owner: "<<buf.msg_perm.gid<<endl;
    cout<<"Effective UID of creator: "<<buf.msg_perm.cuid<<endl;
    cout<<"Effective GID of creator: "<<buf.msg_perm.cgid<<endl;
    cout<<"Mode :"<<buf.msg_perm.mode<<endl;

    msgctl(msgid, IPC_RMID, (struct msqid_ds *) 0 ); 
}