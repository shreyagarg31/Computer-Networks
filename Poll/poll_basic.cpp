#include<stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<poll.h>
#include<iostream>
#include<sys/wait.h>
#include <stdlib.h>
#include<string.h>
using namespace std;
struct pollfd fds[3];
int timeout_msecs = 5000;
int ret;

int main(){
  	int fd[3];
  	char buf[128];
  	fd[0]=fileno(popen("./onec","r"));
  	wait(NULL);
  	sleep(5);
     
  	cout<<"GIVE INPUT FOR TWO:";
  	flush(cout);
  	fd[1]=fileno(popen("./twoc","r"));
  	wait(NULL);
  	sleep(5);
    fd[2] = fileno(popen("./thirdc","r"));
    wait(NULL);
     for(int i=0;i<3;++i){
		fds[i].fd = fd[i];
		fds[i].events = POLLIN;
	}
  	int t=5;
  	while(t--){
  		ret=poll(fds,3,-1);
  		if (ret<=0) {
			printf ("%d seconds elapsed.\n", 0);
			return 0;
		}
    	for(int j=0;j<3;j++){
      		//sleep(1);
      		if(fds[j].revents & POLLIN){
        		int i=read(fds[j].fd,buf,100);
        		write(1,buf,i);
        		//cout<<buf<<endl;
        		//fds[j].revents = 0;
      		}
    	}

    	//for(int i=0;i<3;i++)pclose(fd[i]);
	}
  	return 0;
}