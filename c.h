#ifndef C_h
#define C_h

#define MAXLINE 200
#define MAXNAME 20
#define SERV_PORT 8000

int isIP(char *);			
void *listening();			
void get_name();	
void sendonemsg(char *); 
void startlistening();	

#endif
