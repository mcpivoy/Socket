#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include </home/mcpvioy/Desktop/thread/c.h>

struct sockaddr_in servaddr;
char buf[MAXLINE], receivemsg[MAXLINE];
int sockfd, n;
char IP[INET_ADDRSTRLEN];
int stop = 0;
pthread_t tid;

int main(){
	/* ---ip_addr init--- */
	if(strlen(IP) == 0){
		strcpy(IP, "127.0.0.1");
	}
	
	/* ---connect server--- */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
		perror("Server not responding");
	}
	
	startlistening();
	stop = 1;
	get_name(0);
	while(stop);
	printf("Input 'q' sign out\n");
	while(fgets(buf, MAXLINE, stdin) != NULL){
		buf[strlen(buf) - 1] = 0;
		int quit = 0;
		if((strlen(buf) - 1) == 0){
			if(buf[0] == 'q') quit = 1;
		}
		sendonemsg(buf);
		memset(buf, 0, sizeof(buf));
		if(quit == 1) break;
	}
	close(sockfd);
	return 0;
}

void startlistening(){
	int ret = pthread_create(&tid, NULL, listening, NULL);
	if(ret != 0){
		printf("Fail to create a new thread.");
		exit(0);
	}
	ret = pthread_detach(tid);
	if(ret != 0) exit(0);
}

void *listening(){
	while(1){
		memset(receivemsg, 0, sizeof(receivemsg));
		n = read(sockfd, receivemsg, MAXLINE);
		if (n <= 0){
			printf("Server down\n");
			close(sockfd);
			exit(0);
		}
		else
			printf("%s", receivemsg);
		if(receivemsg[0]!='E') stop = 0;
	}
}

void sendonemsg(char *msg){ 
	write(sockfd, msg, strlen(msg)); 
}

void get_name(){
	char name[MAXNAME];
	memset(buf, 0, sizeof(buf));
	printf("please input your name:\n");
	scanf("%s", name); getchar();
	memset(buf, 0, sizeof(buf));
	strcat(buf, "n@");
	strcat(buf, name); 
	sendonemsg(buf);
}


