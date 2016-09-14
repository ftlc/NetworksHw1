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
#define file_buffer_SIZE 2048

long get_f_len(FILE *fl);

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

        char* file_name = "t_file.txt";

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
                
              //  memset(read_buffer, 0, READ_BUFFER_SIZE - 1);
              //  read(clientSocket, read_buffer, READ_BUFFER_SIZE -1);



                fl = fopen(file_name, "r");
                file_size = get_f_len(fl);

                
                // Read the file into a buffer
                char file_buffer[file_size+1];
                memset(file_buffer, 0, file_size+1);
                fread(file_buffer, file_size, 1, fl);

                // Write the file to the socket
                write(clientSocket, file_buffer, file_size);

                shutdown(clientSocket, SHUT_RDWR);
                close(clientSocket);

        }

}



long get_f_len(FILE *fl)
{
        fseek(fl, 0L, SEEK_END);
        long ret =  ftell(fl);

        rewind(fl);

        return ret;
        

}

