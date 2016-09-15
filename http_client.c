#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

#define TRUE 1
#define FALSE 0


typedef struct _get_r {
        char host[40];
        char filepath[100];
} URL_Struct;


void parseURL(char* url, URL_Struct *s);

int main(int argc, char *argv [])
{
        //Declare Variables 
        int serverSocket;
        struct sockaddr_in serverAddr;
        struct hostent *hp;

        URL_Struct* s = calloc(1, sizeof(URL_Struct));

        unsigned short serverPort;
        char* str_port;
        unsigned int clientLen;
        char* url;
        char* host;
        char* filepath;

        int rtt = FALSE;
        

        // Check for proper argument input
        if(argc < 3 || argc > 4)
        {
                fprintf(stderr, "Usage %s -options URL PORT\n", argv[0]);
                exit(1);
        }

        //Check for "-p" option
        if(!strcmp(argv[1], "-p"))
        {
                rtt = TRUE;
                url = argv[2];
                str_port = argv[3];
        }
        else
        {
                rtt = FALSE;
                url = argv[1];
                str_port = argv[2];
        }

        printf("RTT = %d\n", rtt);
        printf("URL = %s\n", url);
        printf("String Port = %s\n", str_port);

        // Convert Port from string to short
        char *ptr;
        serverPort = strtol(str_port, &ptr, 10);

        printf("Numeric Port = %s\n", str_port);

        parseURL(url, s);

        if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                fprintf(stderr, "Socket error: %s\n", strerror(errno));
                 exit(1);

        }

        
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        hp = gethostbyname(url);

        if(hp == NULL)
        {
                fprintf(stderr, "Error: specified URL does not exist\n");
                exit(1);

        }

        bcopy ( (char*)hp->h_addr, 
                        (char *) &serverAddr.sin_addr.s_addr, 
                        hp->h_length);                
        serverAddr.sin_port = htons(serverPort);


        if(connect(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        {
                fprintf(stderr, "Connect error: %s\n", strerror(errno));
                exit(1);
        }
        else
        {
        printf("Connection Established\n");
        }

        int BUFFER_SIZE = 300;
        char buffer [BUFFER_SIZE];

        char* get_request = malloc(100);
        sprintf(get_request, "GET %s HTTP/1.1\r\n\r\n", s->filepath);

        if(write(serverSocket, get_request, strlen(get_request)) < 0)
        {
                fprintf(stderr, "Write returned an error: %s\n", strerror(errno));
                exit(1);
        }
        printf("Write successful\n");

        memset(buffer, 0, BUFFER_SIZE);

        if(read (serverSocket, buffer, BUFFER_SIZE -1)<0)
        {
                printf("Error reading from socket\n");
                exit(1);

        }
        printf("Read Successful\n");
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);



        while(read (serverSocket, buffer, BUFFER_SIZE - 1))
        {
                printf("%s", buffer);
                memset(buffer, 0, BUFFER_SIZE);
        }

        printf("\nTHIS CODE GOT RUN\n");

        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);

        return 0;

}

void parseURL(char* url, URL_Struct *s)
{
        char* path = malloc(100);
        printf("PATH: %s\n", path);
        char* localpath = strstr(url, "/");
      

        printf("URL: %s\n", url);
        printf("PATH: %s\n", path);

        char* host = malloc(40);
        printf("HOST: %s\n", url);
        printf("PATH: %s\n", path);

        if(localpath == NULL)
        {
                strcpy(s->host, url);
                strcpy(s->filepath, "/");
        }
        else
        {
                strcpy(path, localpath);    
                host = strtok(url, "/");
                strcpy(s->host, host);
                strcpy(s->filepath, path);
        }



        printf("Filepath: %s\n", s->filepath);


}
