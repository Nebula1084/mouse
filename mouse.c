#include "mouse.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int sockfd;
int device_id;
struct sockaddr_in server_addr;
struct hostent *host;

int mouse_init(int d, char* host_name, int port){
	device_id = d;
	if ((host=gethostbyname(host_name))==NULL){
		printf("Gethostname error\n");
		exit(1);
	}
	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1){
		printf("Socket Error:%s\a\n", strerror(errno));
		exit(1);
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);
	if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1){
		printf("Connect Error:%s\a\n", strerror(errno));
		exit(1);
	}
}

struct packet* mouse_login(char* device_secret){
	struct packet* p_login = packet_allocate();
	p_login->message_type = LOGIN;
	packet_put(p_login, device_id);
	packet_put(p_login, (unsigned char*)device_secret, 32);
	send_packet(p_login);
	packet_free(p_login);
	return NULL;
}

struct packet* mouse_report(struct packet* p){
	struct packet* p_report;
	return NULL;
}

struct packet* mouse_control(){
	struct packet* p_control;
	return NULL;
}

struct packet* mouse_logout(){
	struct packet* p_logout = packet_allocate();
	p_logout->message_type = LOGOUT;
	send_packet(p_logout);
	packet_free(p_logout);
	return NULL;
}

int send_packet(struct packet* p){
	unsigned char* buffer;
	buffer = (unsigned char*) malloc(1 + p->content_length);
	if (buffer == NULL){
		printf("send_packet():buffer allocate error!");
	}
	memcpy(buffer, &(p->message_type), 1);
	memcpy(buffer+1, p->payload, p->content_length);
	int i;
	for (i = 0; i< p->content_length; i++){
		printf("%d\n", p->payload[i]);
	}
	send(sockfd, buffer, p->content_length + 1, 0);
	printf("%d byte has been sent.\n", p->content_length + 1);

	free(buffer);
}

struct packet* packet_allocate(){
	struct packet* p;
	p = (struct packet*)malloc(sizeof(struct packet));
	if (p == NULL) {
		printf("packet_allocate():packet allocate error!");
		return NULL;
	}
	p->size = 10;
	p->content_length = 0;
	p->payload = (unsigned char*)malloc(p->size);
	return p;
}

void packet_free(struct packet* p){
	free(p);
}

void packet_put(struct packet* p, int n){
	printf("packet_put():%d\n", n);
	packet_put(p, (unsigned char*)&n, sizeof(n));
}

void packet_put(struct packet* p, float n){
	packet_put(p, (unsigned char*)&n, sizeof(n));
}

void packet_put(struct packet* p, double n){
	packet_put(p, (unsigned char*)&n, sizeof(n));
}

void packet_put(struct packet* p, char n){
	packet_put(p, (unsigned char*)&n, sizeof(n));
}

void packet_put(struct packet* p, unsigned char* buffer, int length){
	int re_alloc = 0;
	while (p->size < p->content_length + length){
		p->size *= 2;
		re_alloc = 1;
	}
	if (re_alloc){
		unsigned char* buffer;
		buffer =(unsigned char*)malloc(p->size);
		memcpy(buffer, p->payload, p->content_length);
		free(p->payload);
		p->payload = buffer;
	}
	int i;
	for (i = 0; i < length; i++){
		p->payload[p->content_length] = buffer[i];
		printf("%d:%d\n", i, buffer[i]);
		p->content_length++;
	}
}
