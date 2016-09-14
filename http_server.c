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


#define READ_BUFFER_SIZE 256
#define FILE_BUFFER_SIZE 5


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
                fprintf(stderr, "Socket: error\n");
        }

        // Initialize Struct
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(serverPort);


        // Bind to the local address
        if (bind (serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <0)
        {
                fprintf(stderr, "Bind: error\n");
        }


        // Listen for incoming connections
        if(listen (serverSocket, SOMAXCONN<0))
        {
                fprintf(stderr, "Listen: error\n");
        }


        //Declare More Variables
        char read_buffer[READ_BUFFER_SIZE];
        long file_size;
        FILE *fl;


        char file_buffer [FILE_BUFFER_SIZE];
        memset(file_buffer, 0, FILE_BUFFER_SIZE);


        char* file_name = "testdir/t_file.txt";

        // Infinite Loop
        // Those are bad, except when they're not
        while(1)
        {
                clientLen = sizeof(clientAddr);

                // Wait for client to connect
                if((clientSocket = accept (serverSocket, (struct sockaddr *) &clientAddr, &clientLen))<0)
                {
                        fprintf(stderr, "Accept: error\n");
                }

                printf("Connection Established\n");

                // Handle Get Request


                // Get the File

                memset(read_buffer, 0, READ_BUFFER_SIZE - 1);
                read(clientSocket, read_buffer, READ_BUFFER_SIZE -1);

                // printf("Example GET request: \n%s", read_buffer);

                // Handle GET request
                
                char* file_location = Handle_Get_Request(read_buffer);
                


                // Open Requested File
                fl = fopen(file_location, "r");

                // Check that file exists
                if(fl == NULL)
                {
                        printf("Requested file could not be opened\n");
                        continue;
                }


                // Read the file into a buffer 

                char file_buffer [FILE_BUFFER_SIZE];
                memset(file_buffer, 0, FILE_BUFFER_SIZE);


                while(fread (file_buffer, 1, 4,  fl) == 4 )
                {
                        write(clientSocket, file_buffer, 4);
                        memset(file_buffer, 0, 4);
                }


                //Close File
                fclose(fl);
                
                //Shutdown Socket
                shutdown(clientSocket, SHUT_RDWR);
                close(clientSocket);



        }
}


char* Handle_Get_Request (char* read_buffer)
{
        char* get_request = strstr(read_buffer, "/");

        printf("\nGet Request:\n%s", get_request);

        if(strncmp(get_request, "/ HTTP/1", 8))
        {

                get_request++;

                char* file_location = strtok(get_request, " ");

                return file_location;

        }

        return "nofile.txt";
}


