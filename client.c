#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 1460 
   
void error_handling(char *message); 
 
int main(int argc, char **argv) 
{ 
  int fp; 
  int sd; 
   
  char buf[BUFSIZE];
  //char cbuf[BUFSIZE]; 
  //int len;
  //int str_len;
  char byte_message[BUFSIZE];
  char hello[20];


  struct sockaddr_in serv_addr; 

  if(argc!=3){ 
    printf("Usage : %s <IP> <port>\n", argv[0]); 
    exit(1); 
  } 
    
 
  
  /* 서버 접속을 위한 소켓 생성 */ 
  sd=socket(PF_INET, SOCK_STREAM, 0);    
  if(sd == -1) 
    error_handling("socket() error"); 
  
  memset(&serv_addr, 0, sizeof(serv_addr)); 
  serv_addr.sin_family=AF_INET; 
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]); 
  serv_addr.sin_port=htons(atoi(argv[2])); 

  if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 ) 
    error_handling("connect() error!"); 

  /*get message and send to server*/
  printf("Message to send: ");
  fgets(hello, sizeof(hello), stdin);
  int hellosize = strlen(hello);
  hello[hellosize - 1] = 0;
  send(sd, hello, sizeof(hello), 0);

	/*read file bytes*/
  int filesize = 0;
	read(sd, &filesize, sizeof(filesize));
  printf("%d", filesize);
	
	
  fp = open("test2.mp4", O_WRONLY|O_CREAT|O_TRUNC); 
  int total = 0;
  int sread = 0;
  /* 데이터를 전송 받아서 파일에 저장한다 */ 
   while( total != filesize) 
   { 	//memset(buf, '\0', BUFSIZE);
      sread = recv(sd, buf, 1460, 0);//100?
      printf("file is receiving now..");
      total += sread;
      buf[sread] = 0;
      write(fp, buf, sread);
      bzero(buf, sizeof(buf));
      //usleep(1);
   } 
   printf("file translating is completed\n");
   printf("filesize : %d, received: %d\n", filesize, total);

   if(chmod("test2.mp4", 00700)){//chmod 777
    fprintf(stderr, "chmod error: %s\n", strerror(errno));
    return -1;
   }

   close(fp); 
   close(sd); 
   return 0; 
} 
  
void error_handling(char *message) 
 { 
      fputs(message, stderr); 
      fputc('\n', stderr); 
      exit(1); 
 }
