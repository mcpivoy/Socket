#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include </home/mcpvioy/Desktop/thread/s.h>

struct sockaddr_in servaddr, clientaddr[MAXCON];
socklen_t cliaddr_len;

int listenfd, connfd[MAXCON], n;
char buf[MAXCON][MAXLINE], spemsg[MAXCON][MAXLINE];
char str[INET_ADDRSTRLEN];
char names[MAXCON][MAXNAME];
int used[MAXCON];

int main(){
	/* ---socket_init---*/
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);
	/* ---data_init--- */
	memset(names, 0, sizeof(names));
	memset(used, 0, sizeof(used));
	
	/*---thread_init---*/
	pthread_t tids[MAXCON];
	int index[MAXCON];
	for(int i = 0; i < MAXCON - 1; i++) index[i] = i; //thread.id
	for(int i = 0; i < MAXCON - 1; i++){
		int ret = pthread_create(&tids[i], NULL, TRD, &index[i]);  //success return 0
		if(ret != 0){
			puts("Thread create failed");
			return 0;
		}
	}
	
	printf("Accepting connections ...\n");
	while(1){
		
		int NowID = 0;
		for(NowID = 0; NowID < MAXCON - 1; NowID++)
			if(!used[NowID]) break;  //occupied or not
		cliaddr_len = sizeof(clientaddr[NowID]);
		connfd[NowID] = accept(listenfd, (struct sockaddr *)&clientaddr[NowID], &cliaddr_len);
		if(NowID >= MAXCON - 1){
			for(NowID = 0; NowID < MAXCON - 1; NowID++)
				if(!used[NowID]) break;
			if(NowID >= MAXCON - 1){
				memset(spemsg[NowID], 0, sizeof(spemsg[NowID]));
				strcpy(spemsg[NowID], "error: The number of people has reached the maximum(2)");
				sendonemsg(connfd[NowID], spemsg[NowID]);
				close(connfd[NowID]);
				continue;
			}else{
				connfd[NowID] = connfd[MAXCON - 1];
				clientaddr[NowID].sin_family = clientaddr[MAXCON-1].sin_family;
				clientaddr[NowID].sin_port = clientaddr[MAXCON-1].sin_port;
				clientaddr[NowID].sin_addr.s_addr = clientaddr[MAXCON-1].sin_addr.s_addr;
			}
		}
		used[NowID] = 1;
	}
	return 0;
}
/* ---send message for sockfd---*/
inline void sendonemsg(int sockfd, char *msg){
	strcat(msg, "\n");
	write(sockfd, msg, strlen(msg));
}
/* ---Encapsulation of sendonemsg---*/
inline void sendmsgtoall(int ID){
	if(strlen(spemsg[ID]) != 0) sendonemsg(connfd[ID], spemsg[ID]);
	for(int i = 0; i < MAXCON - 1; i++){
		if(i == ID) continue;
		else if(used[i]) sendonemsg(connfd[i], buf[ID]);
	}
}
inline int Process(int ID){
	if(buf[ID] == 0) return 0;
	if(buf[ID][0] == 'n' && buf[ID][1] == '@'){
		strcpy(names[ID], buf[ID]);
	}
	char temp[MAXLINE];
	strcpy(temp, buf[ID]);
	sprintf(buf[ID], "\t\n>> %s: %s",names[ID], temp);
	memset(spemsg[ID], 0 , sizeof(spemsg[ID]));
	strcpy(spemsg[ID], "Y0ur InpUt >> ");
	strcat(spemsg[ID], temp);
	strcat(spemsg[ID], "\t\n");
	sendmsgtoall(ID);
	return 0;
}
/* ---recv thread--- */
void *TRD(void *arg){
	int ID = *(int *)arg;
	while(1){
		while(!used[ID]);  
		memset(buf[ID], 0, sizeof(buf[ID]));
		n = read(connfd[ID], buf[ID], MAXLINE);
		if(n <= 0){
			sprintf(buf[ID], "(%s:%d)leave", inet_ntop(AF_INET, &clientaddr[ID].sin_addr, str, sizeof(str)), ntohs(clientaddr[ID].sin_port));
			memset(spemsg[ID], 0, sizeof(spemsg[ID]));
			sendmsgtoall(ID);
			close(connfd[ID]);
			memset(names[ID], 0, sizeof(names[ID]));
			used[ID] = 0; 
		}
		buf[ID][n] = 0;
		printf("Received from PORT %d: %s\n", ntohs(clientaddr[ID].sin_port), buf[ID]);
		int q = Process(ID);
		if(q == 1){
			close(connfd[ID]);
			used[ID] = 0;		
		}
		
	}
}
