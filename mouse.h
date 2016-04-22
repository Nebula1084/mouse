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

typedef struct packet {
	unsigned char message_type;
	unsigned char* payload;
	int size;
	int content_length;
}packet;

typedef packet* (*packingfunc)(packet*, void*);

#ifdef __cplusplus
extern "C" {
#endif

int mouse_init(int device_id, char* host_name, int port);
packet* mouse_login(char* device_secret);
packet* mouse_report(packingfunc func, void* data);
packet* mouse_control();
packet* mouse_logout();

packet* packet_allocate();
void packet_free(packet* p);
int send_packet(packet* p);
void packet_put_int(packet* p, int n);
void packet_put_float(packet* p, float n);
void packet_put_double(packet* p, double n);
void packet_put_char(packet* p, char n);
void packet_put_buffer(packet* p, unsigned char* buffer, int length);

#ifdef __cplusplus
}
#endif

#endif /* _MOUSE_H */
