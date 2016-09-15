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


int main(int argc, char *argv [])
{
        //Declare Variables 
        int serverSocket;
        struct sockaddr_in serverAddr;
        struct hostent *hp;

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

        printf("Numeric Port = %s\n", str_port);

        if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                fprintf(stderr, "Socket error: %s\n", strerror(errno));
        }

        
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        hp = gethostbyname(url);

        if(hp == NULL)
        {
                fprintf(stderr, "Error: specified URL does not exist\n");
        }

        bcopy ( (char*)hp->h_addr, 
                        (char *) &serverAddr.sin_addr.s_addr, 
                        hp->h_length);                
        serverAddr.sin_port = htons(serverPort);


        if(connect(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        {
                fprintf(stderr, "Connect error: %s\n", strerror(errno));
        }

        printf("Connection Established\n");


        int BUFFER_SIZE = 300;
        char buffer [BUFFER_SIZE];

        if(write(serverSocket, "GET / HTTP/1.1\r\n\r\n", strlen("GET / HTTP/1.1\r\n\r\n")) < 0)
        {
                fprintf(stderr, "Write returned an error: %s\n", strerror(errno));
                exit(1);
        }
        printf("Write successful\n");

        bzero(buffer, BUFFER_SIZE);

        if(read (serverSocket, buffer, BUFFER_SIZE -1)<0)
        {
                printf("Error reading from socket\n");
        }
        printf("Read Successful\n");
        printf("%s", buffer);
        bzero(buffer, BUFFER_SIZE);



        while(read (serverSocket, buffer, BUFFER_SIZE -1))
        {
                printf("%s", buffer);
                bzero(buffer, BUFFER_SIZE);
        }

        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);

        return 0;

}
