#include <stdio.h> 
//#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<linux/msg.h>
using namespace std;

struct mymsg{
	long type;
	char mdata[512];
};

struct msg {
    struct msg* msg_next;   
    long  msg_type;          
    char msg_spot;         
    short msg_ts;          
};
int main()
{
	struct mymsg msg1,msg2;
	/*struct mymsg chkmq[5];
    chkmq[0].type=1;
    strcpy(chkmq[0].mdata,"cse");
    chkmq[1].type=2;
    strcpy(chkmq[1].mdata,"goa");
    chkmq[2].type=3;
    strcpy(chkmq[2].mdata,"nitw");
    chkmq[3].type=1;
    strcpy(chkmq[3].mdata,"CN");
    chkmq[4].type=2;
    strcpy(chkmq[4].mdata,"2");*/
	key_t key1 = ftok(".",65);
	int msgid = msgget(key1,0666|IPC_CREAT);
	char msgchk[10][10] = {"1","cse","2","nitw","3","csea","1","CN","2","Goa"};
    for(int i=0;i<9;i+=2)
    {
    	msg1.type = atol(msgchk[i]);
    	strcpy(msg1.mdata,msgchk[i+1]);
    	msgsnd(msgid,&msg1,sizeof(msg1.mdata),0);
    	cout<<"Sent message: "<<msg1.mdata<<endl;
    }

    /*for(int i=0;i<5;i++){
        msgsnd(msgid,&chkmq[i],sizeof(chkmq[i]),0);
    }*/

    struct msqid_ds buff;
	msgctl(msgid,IPC_STAT,&buff);
	cout<<"No of messages in queue : "<<buff.msg_qnum<<endl;
	cout<<"No of bytes of queue : "<<buff.msg_cbytes<<endl;
	cout<<buff.msg_first->msg_spot<<endl;

    
    int c=1;
    for(int i=0;i<5;i++)
    {
    	while(msgrcv(msgid,&msg2,sizeof(msg2.mdata),c,IPC_NOWAIT)!=-1)
    		cout<<"Message received : "<<msg2.mdata<<endl;
        //printf("Message received is : %s with type: %d \n",msg2.mdata,c);
        c++;
    }
    msgctl(msgid,IPC_STAT,&buff);
    cout<<"No of messages in queue : "<<buff.msg_qnum<<endl;
	cout<<"No of bytes of queue : "<<buff.msg_cbytes<<endl;

    msgctl(msgid,IPC_RMID,(struct msqid_ds *) 0 );
    return 0;

}