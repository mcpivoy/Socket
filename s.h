#ifndef S_H
#define S_H

#define MAXLINE 100
#define MAXNAME 20
#define SERV_PORT 8000
#define MAXCON 3

void *TRD(void *);
int Process(int);
void sendonemsg(int, char *);
void sendmsgtoall(int );

#endif
