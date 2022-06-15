all: Server Client
.PHONY : all

Server:
	gcc -o Server Server.c -lpthread

Client:
	gcc -o Client Client.c -lpthread

.PHONY : clean
clean :
	rm Client Server 
	
