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

#define BUF_SIZE 100

void *get_sockin(struct sockaddr *s);

int main(int argc, char *argv [])
{

        struct addrinfo hints, *r; //short for rotate
        struct addrinfo *res;

        char cli[INET6_ADDRSTRLEN];

        int ret;

        if(argc < 3) //make sure at least 2 arguments are given
        {
                fprintf(stderr, "Too few arguments.\n");
                return 1;
        }

        //hints = (struct addrinfo *) malloc (sizeof(struct addrinfo));       
        memset(&hints, 0, sizeof(struct addrinfo)); //initialize hints to 0


        hints.ai_family = AF_INET; //ip version agnostic
        hints.ai_socktype = SOCK_STREAM; //TCP instead of UDP
        hints.ai_flags = AI_PASSIVE; //
       


        if(getaddrinfo(argv[1], argv[2], &hints, &res) < 0)
        {
                fprintf(stderr, "Getaddrinfo returned an error\n");
                return 1;
        }

        int cnt = 0;

        int f_descriptor; //file descripter returned by socket()
        for (r = res; r != NULL; r = r->ai_next) //rotate through linked list until connection can be established
        {
                if(f_descriptor = socket(r->ai_family, r->ai_socktype, 0) == -1) // if socket returns error, continue to next iteration
                {
                        perror("client: socket not connected\n");
                        continue; 
                }

                if(connect(f_descriptor, r->ai_addr, r->ai_addrlen)) //if connect returns error, continue to next iteration
                {
                        cnt++;
                        close(f_descriptor);
                        printf("Attempt # %d\n", cnt);
                        perror("client: connect not connected \n");
                        continue;
                }


                break; //if no erros, break out of loop
                // seems like the standard way to do it
        }
        if(r == NULL) //if never broke out of loop, then connection wasn't established
        {
                fprintf(stderr, "Failed to connect.\n");
                return 1;
        }



        inet_ntop(r->ai_family, get_sockin((struct sockaddr *)r->ai_addr), cli, sizeof cli );

        printf("client: conecting to %s\n", cli);

        freeaddrinfo(res);

        int BUFFER_SIZE = 2048;
        char buffer [BUFFER_SIZE];

        write(f_descriptor, "GET \r\n", strlen("GET \r\n"));

        bzero(buffer, BUFFER_SIZE);

        while(read (f_descriptor, buffer, BUFFER_SIZE -1))
        {
                printf("%s", buffer);
                bzero(buffer, BUFFER_SIZE);
        }



        shutdown(f_descriptor, SHUT_RDWR);
        close(f_descriptor);



        return 0;
}






void *get_sockin(struct sockaddr *s)
{
        if(s->sa_family == AF_INET)
        {
                return &(((struct sockaddr_in*)s)->sin_addr); // cast as sockaddr_in, get the field, and return
        }
        else
        {
                return &(((struct sockaddr_in6*)s)->sin6_addr);  // cast as sockaddr_in6, get the field, and return

        }
}
