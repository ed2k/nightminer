#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3032); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
	struct hostent *hp = gethostbyname(argv[1]);
	if (hp == NULL) {
		return 1;
	}
        struct in_addr **addr_list = (struct in_addr **) hp->h_addr_list;
	char ip[100];         
        for(int i = 0; addr_list[i] != NULL; i++) {
	//Return the first one;
	        strcpy(ip , inet_ntoa(*addr_list[i]) );
	}
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
        printf("\n inet_pton error occured\n");
        return 1;
	}
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    char *m = "{\"params\": [\"NightMiner/0.1\"], \"method\": \"mining.subscribe\", \"id\": 1}";
    send(sockfd, m, strlen(m),0);
    fputs(m, stdout);
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}
