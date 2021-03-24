#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>

#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
int v[500];

//char *socket_path = "./socket";
char *path = "./sockp";

int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 

int recv_fd(int usfd) {
    struct iovec iov[1];
    char buf[2];
    int newfd;

    struct msghdr msg;

    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    cmptr = malloc(CONTROLLEN);
    msg.msg_control = cmptr;
    msg.msg_controllen = CONTROLLEN;

    if(recvmsg(usfd, &msg,MSG_DONTWAIT) < 0) {
        perror("recvmsg");
        return -1;
    }

    newfd = *(int *)CMSG_DATA(cmptr);

    return newfd;
}

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    int usfd,rc,j=0;

    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    if(connect(usfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(1);
    }

    fd_set rset;
    FD_ZERO(&rset);
    int maxfd = usfd + 1;
    while(1)
    {
        FD_SET(usfd, &rset);
        for(int i=0;i<j;i++){maxfd = max(v[i],maxfd); FD_SET(v[i],&rset);}
        maxfd = max(maxfd,usfd) + 1;
        int nready = select(maxfd, &rset, NULL, NULL, NULL); 
        if (FD_ISSET(usfd, &rset)) { 
            int rfd = recv_fd(usfd);
            //v.push_back(rfd);
            v[j++]=rfd;
            char buff[1024];
            read(rfd, buff, 1024);
            printf("%s", buff);
            fflush(stdout);
        } 
        for(int i=0;i<j;i++)
        {
            if(FD_ISSET(v[i],&rset))
            {
                char buff[1024];
                read(v[i], buff, 1024);
                printf("%s", buff);
                fflush(stdout);
            }
        }
    }
    /*while(1){
    int rfd;
    if((rfd = recv_fd(usfd)) < 0) {
        perror("recv_fd");
        exit(1);
    }
    char buff[1024];
    read(rfd, buff, 1024);
    printf("%s", buff);
    fflush(stdout);
    }*/
    close(usfd);
    return 0;
}