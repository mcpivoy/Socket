#ifndef SERVER_H
#define SERVER_H

#define MAXLINE 100    //服务器单次发送的最大长度 可改
#define MAXNAME 20     //用户名最大长度 可改  请同步修改client.h长度 
#define SERV_PORT 8000
#define MAXCON 3   //最多可连接用户数  client_num = (MAXCON - server_num)  = 2

void *TRD(void *);
int Process(int);
void sendonemsg(int, char *);
void sendmsgtoall(int );

#endif
