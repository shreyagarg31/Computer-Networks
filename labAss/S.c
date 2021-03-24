#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 37777
#define PORT2 37778
#define PORT3 7001 
#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;

char *path = "./sockp";

int send_fd(int usfd, int fd_to_send) {
    
    struct iovec iov[1];
    struct msghdr msg;
    
    char buf[2] = {0,0};
    /* buf[0] = 0 means null byte flag to recv_fd() 
        buf[1] = 0 means zero status implies OK */
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    
    cmptr = malloc(CONTROLLEN);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    cmptr->cmsg_len = CONTROLLEN;
    msg.msg_control = cmptr;
    msg.msg_controllen = CONTROLLEN;
    *(int *)CMSG_DATA(cmptr) = fd_to_send;

    if(sendmsg(usfd, &msg, 0) < 0) {
        perror("sendmsg");
        return -1;
    }

    return 0;
}

int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 

int main()
{
	int sfd, nsfd, nsfd2 , nready, maxfdp1; 
    char buffer[1024]; 
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 
    char* message = "Hello Client"; 
    
    /* create listening TCP socket */
    sfd = socket(AF_INET, SOCK_STREAM, 0); 
    int sfd3 = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    bind(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 


    servaddr.sin_port = htons(PORT2); 
    bind(sfd3, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(sfd, 10); 
    listen(sfd3,10);

    /* create UDP socket */
    int sfd2 = socket(AF_INET, SOCK_DGRAM, 0); 
    servaddr.sin_port = htons(PORT3);
    bind(sfd2, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

    FD_ZERO(&rset);  //clear reading set
    int maxfd = max(sfd, sfd3); 
    maxfdp1 = max(maxfd,sfd2)+1;

    //for fd passing unix socket

    struct sockaddr_un addr;
    char buf[1024];
    int usfd,cl,rc,nufd;

    if ( (usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    unlink(path);
    
    if (bind(usfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(1);
    }

    if (listen(usfd, 5) == -1) {
        perror("listen error");
        exit(1);
    }
    struct sockaddr cli_addr;
    int leng;
    if((nufd = accept(usfd, (struct sockaddr*)&cli_addr, &leng)) < 0) {
        perror("accept error");
        exit(1);
    }

    while(1)
    {
    	FD_SET(sfd, &rset); 
        FD_SET(sfd3, &rset);  //set fds in read_set
        FD_SET(sfd2,&rset);
        
        // select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
       // if tcp socket is readable then handle 
        // it by accepting the connection 
        if (FD_ISSET(sfd, &rset)) { 
            len = sizeof(cliaddr); 
            nsfd = accept(sfd, (struct sockaddr*)&cliaddr, &len); 
            send_fd(nufd,nsfd);
        } 

        if (FD_ISSET(sfd3, &rset)) { 
            len = sizeof(cliaddr); 
            nsfd2 = accept(sfd3, (struct sockaddr*)&cliaddr, &len); 
            printf("Going to send!\n");
            send_fd(nufd,nsfd2);
        } 

        // if udp socket is readable receive the message. 
        if (FD_ISSET(sfd2, &rset)) { 
            len = sizeof(cliaddr); 
            bzero(buffer, sizeof(buffer)); 
            printf("\nMessage from UDP client: "); 
            n = recvfrom(sfd2, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&cliaddr, &len); 
            //puts(buffer);
            printf("%s\n",buffer); 
            sendto(sfd2, (const char*)message, sizeof(buffer), 0, 
                   (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
        } 
    }
}