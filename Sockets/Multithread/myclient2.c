#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT1 43887
struct sockaddr_in serv_addr;
int main()
{
	char buf[1024]; 
	int csfd;
	if ((csfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   serv_addr.sin_family = AF_INET; 
   serv_addr.sin_port = htons(PORT1); 
   if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(csfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    while(1){
    printf("Enter input for client 2:\n");
    fgets(buf,sizeof(buf),stdin);
    send(csfd , buf , strlen(buf)+1 , 0 ); 
    printf("Message sent!\n");
    }
    return 0;
}