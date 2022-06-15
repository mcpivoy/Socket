#ifndef CLIENT_H
#define CLIENT_H

#define MAXLINE 200
#define MAXNAME 20
#define SERV_PORT 8000

int isIP(char *);			
void *listening();			
void get_name();	
void sendonemsg(char *); 
void startlistening();	

#endif
