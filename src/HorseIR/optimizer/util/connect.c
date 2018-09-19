#include "../global.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 * Source:
 *   http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 *   https://www.geeksforgeeks.org/socket-programming-cc/
 */
int runModeServer(){
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        EP("Fail in opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8011; //default
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        EP("Fail in binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(true){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) EP("Fail in accept");
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) EP("Fail in reading from socket");
        if(!strcmp(buffer, "q")) break;
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"Server got your message",23);
        if (n < 0) EP("Fail in writing to socket");
    }
    return 0; 
}

int runModeClient()
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = 8011; //default
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        EP("Fail in opening socket");
    server = gethostbyname("localhost");
    if (server == NULL) EP("No such host\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        EP("Fail in connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    int len = strlen(buffer);
    if(len>0 && buffer[len-1]=='\n') buffer[--len]=0;
    n = write(sockfd,buffer,len);
    if (n < 0) 
         EP("Fail in writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         EP("Fail in reading from socket");
    printf("%s\n",buffer);
    return 0;
}
