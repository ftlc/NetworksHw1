#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>


// Pre-processor definitions
#define READ_BUFFER_SIZE 256
#define FILE_BUFFER_SIZE 5


//Function Definitions
char* Handle_Get_Request (char* read_buffer);



int main(int argc, char* argv[])
{
        // Declare Variables
        int serverSocket;
        int clientSocket;

        struct sockaddr_in serverAddr;
        struct sockaddr_in clientAddr;

        unsigned short serverPort;
        unsigned int clientLen;

        // Check for correct input
        if(argc != 2)
        {
                fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
                exit(1);

        }

        // Connect
        char *ptr;
        serverPort = strtol(argv[1], &ptr, 10);

        // First we create the socket

        if((serverSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
                fprintf(stderr, "Socket: error%s\n", strerror(errno));
        }

        // Initialize Struct
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(serverPort);


        // Bind to the local address
        if (bind (serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <0)
        {
                fprintf(stderr, "Bind: error%s\n", strerror(errno));
        }


        // Listen for incoming connections
        if(listen (serverSocket, SOMAXCONN<0))
        {
                fprintf(stderr, "Listen: error%s\n", strerror(errno));
        }


        //Declare More Variables
        char read_buffer[READ_BUFFER_SIZE];
        long file_size;
        FILE *fl;


        char file_buffer [FILE_BUFFER_SIZE];
        memset(file_buffer, 0, FILE_BUFFER_SIZE);



        // Infinite Loop
        // Those are bad, except when they're not
        while(1)
        {

                clientLen = sizeof(clientAddr);

                // Wait for client to connect
                if((clientSocket = accept (serverSocket, (struct sockaddr *) &clientAddr, &clientLen))<0)
                {
                        fprintf(stderr, "Accept: error%s\n", strerror(errno));
                }

                printf("Connection Established\n");



                // Get the File

                memset(read_buffer, 0, READ_BUFFER_SIZE - 1);
                read(clientSocket, read_buffer, READ_BUFFER_SIZE -1);

                printf("GET request: \n%s", read_buffer);

                
                
                // Handle GET request
                char* file_location = Handle_Get_Request(read_buffer);


                // Open Requested File
                fl = fopen(file_location, "r");

                // Check that file exists
                if(fl == NULL)
                {
                        char nofile [14] = "404 Not Found\n";
                        fprintf(stderr, nofile);
                        write(clientSocket, nofile, sizeof(nofile) -1);
                }
                else
                {


                        // Read the file into a buffer 

                        char file_buffer [FILE_BUFFER_SIZE];
                        memset(file_buffer, 0, FILE_BUFFER_SIZE);


                      //  send(clientSocket, "HTTP/1.1 200 OK\n", sizeof("HTTP/1.1 200 OK\n"), 0);
                        // Write the file to the client socket
                        while(fread (file_buffer, 1, 4,  fl) == 4 )
                        {
                                write(clientSocket, file_buffer, 4);
                                memset(file_buffer, 0, 4);
                        }



                        //Close File
                        fclose(fl);

                }

                //Shutdown Socket
                shutdown(clientSocket, SHUT_RDWR);
                close(clientSocket);



        }



}


// Handle the get request
char* Handle_Get_Request (char* read_buffer)
{
        //Get everything after the first slash
        char* get_request = strstr(read_buffer, "/");


        //Check for filepath given. Return default index otherwise
        if(strncmp(get_request, "/ HTTP/1", 8))
        {

                // Iterate string pointer
                // gets rid of leading slash
                get_request++;

                //parse out filepath 
                //everything before the first space
                char* file_location = strtok(get_request, " ");

                // return filepath
                return file_location;

        }

        return "index.html";
}


