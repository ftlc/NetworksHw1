Harutyun Sadoyan 
hsadoyan@wpi.edu


ReadMe for Porject 1

Compiling Instructions:
        
        Makefile is present. 

        Simply run "make" to compile both executables.


Running the Code:

There are two executables: "http_server" and "http_client"

http_server: 
        Usage: ./http_server [PORT]

        Runs simple TCP server on the specified port. 

        Writes the requested file to the client. If no files are requested, uses "index.html" as the default file.

        Gracefully closes client socket once finished and listens for further connections.


http_client:
        Usage: ./http_client [-p] [www.url.com/file_path] [PORT]

        Is a simple TCP client able to connect to any open webserver and request a file

        -p option will cause it to print out RTT statistics.

        Use port "80" for well known websites or custom specified port for custom servers.

        Will gracefully shut down the socket after it's finished. 
