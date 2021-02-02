#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc,char *argv[])
{
    char buf1[128],buf2[128];
    int fd1=dup(STDIN_FILENO);
    int fd2=dup(STDOUT_FILENO);
    dup2(7,STDIN_FILENO);
    dup2(8,STDOUT_FILENO);
    read(fd1,buf1,128);
    printf("Read from pipe1: %s",buf1);
    printf("Writing to Pipe2:");
    fgets(buf2,128,stdin);
    write(fd2,buf2,128);
    close(fd1);
    close(fd2);
    return 0;
}