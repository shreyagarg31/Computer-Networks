#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include<stdlib.h>
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#define PORT1 43889
#define PORT2 43887

struct sockaddr_in serv_addr, cli_addr;
pthread_t thr1,thr2;

void* func(void* args)
{
    int* fd=(int*)args;
    int nsfd=*fd;
    printf("%d",nsfd);
    while(1){
    char buf[1024];
    printf("%d",nsfd);
    int val = read(nsfd,buf,sizeof(buf));
    if(val==0)
    {
        close(nsfd);
        return 0;
    }
    printf("Received msg: %s\n",buf);
    fflush(stdout);
  }
}



int main()
{
  //address config
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT1);

    //socket creation
    int sfd1,sfd2;
     if((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
	if((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    //setsockopt to reuse the same address
    int addrlen = sizeof(cli_addr); 
    int opt = 1;
    if((setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
        perror("setsockopt");
        exit(1);
    }
	if((setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
        perror("setsockopt");
        exit(1);
    }
    //bind address to socket fd
    if(bind(sfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(1);
    }
	serv_addr.sin_port = htons(PORT2);
	if(bind(sfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(1);
    }
    if(listen(sfd1, 5) < 0) {
        perror("listen");
        exit(1);
    }
    if(listen(sfd2, 5) < 0) {
        perror("listen");
        exit(1);
    }
    printf("Server started\n");	
    while(1)
    {
    	struct pollfd fds[2];
    	fds[0].fd = sfd1;
        fds[0].events = POLLIN;
		fds[0].revents = 0;
		fds[1].fd = sfd2;
        fds[1].events = POLLIN;
		fds[1].revents = 0;
        
        poll(fds, 2, -1);
        if(fds[0].revents &POLLIN)
        {
            int nsfd;
	    	if((nsfd = accept(sfd1, (struct sockaddr *)&cli_addr,  
	                       (socklen_t*)&addrlen))<0) 
		    { 
		        perror("accept"); 
		        exit(1); 
		    } 
            printf("New Client connected!\n");
            fflush(stdout);
            pthread_create(&thr1,NULL,&func,(void*)&nsfd);
            //pthread_join(thr1,NULL);
            //pthread_cancel(thr1);
        }

        if(fds[1].revents & POLLIN)
        {
        	int nsfd;
	    	if((nsfd = accept(sfd2, (struct sockaddr *)&cli_addr,  
	                       (socklen_t*)&addrlen))<0) 
		    { 
		        perror("accept"); 
		        exit(1); 
		    } 
		    printf("New Client connected!\n");
            fflush(stdout);
            pthread_create(&thr2,NULL,&func,(void*)&nsfd);
            //pthread_join(thr2,NULL);
            //pthread_cancel(thr2);
        }
    }
}