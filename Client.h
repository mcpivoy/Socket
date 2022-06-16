#ifndef CLIENT_H
#define CLIENT_H

#define MAXLINE 200    //用户单次输入的最大长度 可改
#define MAXNAME 20     //用户名最大长度 可改
#define SERV_PORT 8000
		
void *listening();			
void get_name();	
void sendonemsg(char *); 
void startlistening();	

#endif
