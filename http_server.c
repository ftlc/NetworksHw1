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

// MAKING COMMENTS 
long get_f_len(FILE *fl);

int main(int argc, char* argv[])
{
        int servSock;                    /*Socket descriptor for server */
        int clntSock;                    /* Socket descriptor for client */
        struct sockaddr_in servAddr; /* Local address */
        struct sockaddr_in clntAddr; /* Client address */
        unsigned short servPort;     /* Server port */
        unsigned int clntLen;            /* Length of client address data structure */ 

        if (argc != 2)     /* Test for correct number of arguments */
        { 
                fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
                exit(1);   
        }
        servPort = atoi(argv[1]);  /* First arg:  local port */

        printf("Test 0\n");
        /* Create socket for incoming connections */
        if ((servSock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {       //{         DieWithError("socket() failed");  
                fprintf(stderr, "stderr: socket\n");
        }

        printf(" TEST 1\n");


        memset(&servAddr, 0, sizeof(servAddr));


        servAddr.sin_family = AF_INET;                              /* Internet address family */
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
        servAddr.sin_port = htons(servPort);                 /* Local port */

        /* Bind to the local address */
        if (bind (servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        {

                fprintf(stderr, "stderr: bind\n");
        }//DieWithError("bind() failed");

        printf(" Test 2\n");

        
        /* Mark the socket so it will listen for incoming connections */
        if (listen (servSock, SOMAXCONN < 0))
        {
                fprintf(stderr, "stderr: listn\n");
        }//     DieWithError("listen() failed");
        int BUFFER_SIZE = 128;
        char buffer[BUFFER_SIZE];

        printf("Test 3\n");

        for (;;) /* Run forever */
        {
                /* Set the size of the in-out parameter */
                clntLen = sizeof(clntAddr);        /* Wait for a client to connect */
                if ((clntSock = accept (servSock, (struct sockaddr *) &clntAddr, &clntLen))<0) 
                {
                        
                fprintf(stderr, "stderr: accept\n");
                }//                DieWithError("accept() failed");

                
              printf("Test 4\n"); 
                
                // GET Request
                //
                FILE *fl = fopen("TMDG.txt", "r");

                printf("Test 4.5\n");
                long file_size = get_f_len(fl);

                printf("Test 5\n");
                

                bzero(buffer, BUFFER_SIZE);
                read(clntSock, buffer, BUFFER_SIZE-1);


                char fl_buffer [file_size + 1];
                bzero(fl_buffer, file_size + 1);
                fread(fl_buffer, file_size, 1, fl);

                write(clntSock, fl_buffer, file_size);

                

                shutdown(clntSock, SHUT_RDWR);
                close(clntSock);



        }
        /* NOT REACHED */
} 

// Get file length


long get_f_len(FILE *fl)
{
        printf("Test GET_F 1\n");
        fseek(fl, 0L, SEEK_END);
        printf("Test GET_F 2\n");
        long ret =  ftell(fl);

        rewind(fl);

        return ret;

        

        return 100;
}

