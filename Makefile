all: http_client http_server 

http_server: http_server.c
	gcc -g http_server.c -o http_server

http_client: http_client.c
	gcc -g http_client.c -o http_client

#c_server: c_server.c
#	gcc -g c_server.c -o c_server

#c_client: c_client.c
#	gcc -g c_client.c -o c_client
