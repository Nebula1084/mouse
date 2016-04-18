#ifndef _MOUSE_H
#define _MOUSE_H

enum MESSAGE_TYPE {
	ACK = 0,
	NACK = 1,
	LOGIN = 2,
	REPORT = 3,
	CONTROL = 4,
	LOGOUT = 5
};

struct packet {
	unsigned char message_type;
	unsigned char* payload;
	int size;
	int content_length;
};

int mouse_init(int device_id, char* host_name, int port);
struct packet* mouse_login(char* device_secret);
struct packet* mouse_report(struct packet* p);
struct packet* mouse_control();
struct packet* mouse_logout();

struct packet* packet_allocate();
void packet_free(struct packet* p);
int send_packet(struct packet* p);
void packet_put(struct packet* p, int n);
void packet_put(struct packet* p, float n);
void packet_put(struct packet* p, double n);
void packet_put(struct packet* p, char n);
void packet_put(struct packet* p, unsigned char* buffer, int length);

#endif
