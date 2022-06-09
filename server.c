#define _CRT_SECURE_NO_WARNINGS
#include <netdb.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h> 
 
#define BUFSIZE 1460
 
void error_handling(char *message); 
 
int main(int argc, char **argv) 
{   
    int serv_sd; 
    int clnt_sd; 
    int fp;
    char buf[BUFSIZE];
    //char cbuf[BUFSIZE];
    struct sockaddr_in serv_addr; 
    struct sockaddr_in clnt_addr; 
    int clnt_addr_size; 
    //int len; 
	//int str_len;
    char hello[20];
     
    if(argc!=2){ 
        printf("Usage : %s <port>\n", argv[0]); 
        exit(1); 
    } 
     
    serv_sd=socket(PF_INET, SOCK_STREAM, 0);    
    if(serv_sd == -1) 
        error_handling("socket() error"); 
     
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); 
    serv_addr.sin_port=htons(atoi(argv[1])); 
     
    if( bind(serv_sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ) 
        error_handling("bind() error"); 
     
    if( listen(serv_sd, 5)==-1 ) 
        error_handling("listen() error"); 
     
    clnt_addr_size=sizeof(clnt_addr);     
    clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_addr,&clnt_addr_size); 
    if(clnt_sd==-1) 
        error_handling("accept() error");

    /*receive message from client*/
    bzero(hello, 20);
    recv(clnt_sd, hello, sizeof(hello), 0);
    printf("Hello Client");

    /* 접속해오는 클라이언트에게 전송해줄 파일 오픈  */ 
    fp = open( "test.mp4", O_RDONLY );  
    if(fp == -1) 
        error_handling("File open error!"); 

    
    /*read file bytes and send to client*/
    int filesize = 0;
    filesize = lseek(fp, 0, SEEK_END);
    send(clnt_sd, &filesize, sizeof(filesize), 0);
    lseek(fp, 0, SEEK_SET);

    /* 파일을 전송 */
    int total = 0;
    int sread = 0;
    while(total != filesize){
        sread = read(fp, buf, 1460);
        printf("file is sending now");
        total += sread;
        buf[sread] = 0;
        send(clnt_sd, buf, sread, 0);
        //usleep(1);
    }
    printf("file translating is completed\n");
    printf("filesize : %d, sending %d\n", filesize, total);

   close(fp); 
   close(clnt_sd); 
   return 0; 
} 
 
void error_handling(char *message) 
{ 
    fputs(message, stderr); 
    fputc('\n', stderr); 
    exit(1); 
}
