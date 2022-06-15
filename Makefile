all: Server Client
.PHONY : all

Server:
	cc -o Server Server.c -lpthread

Client:
	cc -o Client Client.c -lpthread

.PHONY : clean
clean :
	rm Client Server 
	
