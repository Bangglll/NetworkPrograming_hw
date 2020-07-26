#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int i;
    int sock;
    char message[BUF_SIZE];
    struct hostent *host;
    struct sockaddr_in serv_adr;
    int idx=0, read_len=0;
    if(argc!=3){
        printf("Usage : %s <name><port>\n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if(!host)
        error_handling("gethost... error");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = (host->h_addrtype==AF_INET)?AF_INET:AF_INET6;
    serv_adr.sin_addr.s_addr=inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");

    write(sock, "GET /webhp HTTP/1.1\r\nUser-Agent: Mozilla/4 0\r\ncontent-type:text/html\r\nConnection: close\r\n\r\n", strlen("GET /webhp HTTP/1.1\r\nUser-Agent: Mozilla/4 0\r\ncontent-type:text/html\r\nConnection: close\r\n\r\n"));

    printf("\n");
    read_len = read(sock, message, BUF_SIZE);
    while(1){
	printf("%s", message);
        memset(message,0,BUF_SIZE);
        read_len = read(sock, message, read_len);
	if(read_len==EOF||read_len==0) break;
    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
