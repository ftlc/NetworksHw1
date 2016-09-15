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

// Preprocessor definitions
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 300


// Define struct for host and path
typedef struct _get_r {
        char host[40];
        char filepath[100];
} URL_Struct;


// Function definitions
void parseURL(char* url, URL_Struct *s);

// Main
int main(int argc, char *argv [])
{
        //Declare Variables 
        int serverSocket;
        struct sockaddr_in serverAddr;
        struct hostent *hp;

        // Allocate memory for struct
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

        // Parse url. Divide into host and filepath. Store in the struct
        parseURL(url, s);


        // Socket
        if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                fprintf(stderr, "Socket error: %s\n", strerror(errno));
                exit(1);

        }

        // Initialize struct
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        hp = gethostbyname(url);

        //Check for valid url
        if(hp == NULL)
        {
                fprintf(stderr, "Error: specified URL does not exist\n");
                exit(1);

        }

        bcopy ( (char*)hp->h_addr, 
                        (char *) &serverAddr.sin_addr.s_addr, 
                        hp->h_length);                
        serverAddr.sin_port = htons(serverPort);


        // Connect to server socket
        if(connect(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        {
                fprintf(stderr, "Connect error: %s\n", strerror(errno));
                exit(1);
        }
        else
        {
                printf("Connection Established\n");
        }


        // Initialize read buffer
        //  int BUFFER_SIZE = 300;
        char buffer [BUFFER_SIZE];



        // Print GET request to variable
        char* get_request = malloc(100);
        sprintf(get_request, "GET %s HTTP/1.1\r\n\r\n", s->filepath);

        // Write request to server socket
        if(write(serverSocket, get_request, strlen(get_request)) < 0)
        {
                fprintf(stderr, "Write returned an error: %s\n", strerror(errno));
                exit(1);
        }
        printf("Write successful\n");

        // Free allocated memory
        free(s);

        // Zero the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Check that for successfult read
        if(read (serverSocket, buffer, BUFFER_SIZE -1)<0)
        {
                printf("Error reading from socket\n");
                exit(1);

        }
        printf("Read Successful\n");
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);

        // Read the rest of the buffer
        while(read (serverSocket, buffer, BUFFER_SIZE - 1))
        {
                printf("%s", buffer);
                memset(buffer, 0, BUFFER_SIZE);
        }



        printf("\nTHIS CODE GOT RUN\n");

        // Shutdown and close server socket
        shutdown(serverSocket, SHUT_RDWR);
        close(serverSocket);

        return 0;

}

// Parse URL from user imput
void parseURL(char* url, URL_Struct *s)
{
        // Declare variables
        char* path = malloc(100);

        // temp path variable to avoid issues with pointer being iterated 
        char* localpath = strstr(url, "/");
        char* host = malloc(40);

        // check for filepath
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
