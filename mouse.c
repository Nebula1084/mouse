#include "mouse.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef NDEBUG
#define dbg_print(format, args...) \
    fprintf(stdout, format, ##args)
#else
#define dbg_print(format, args...)
#endif /* NDEBUG */

#define PAYLOAD_INITALLOC_SIZE 16

static int sockfd;
static int device_id;
static struct sockaddr_in server_addr;
static struct hostent* host;

int mouse_init(int d, char* host_name, int port)
{
    device_id = d;
    if ((host = gethostbyname(host_name)) == NULL) {
        fprintf(stderr, "Gethostname error\n");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        exit(1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    if (connect(sockfd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
        exit(1);
    }
    return 0;
}

packet* mouse_login(char* device_secret)
{
    packet* p_login = packet_allocate();
    dbg_print("---Preparing Login packet.---\n");
    p_login->message_type = LOGIN;
    packet_put_int(p_login, device_id);
    packet_put_buffer(p_login, (unsigned char*)device_secret, 32);
    dbg_print("---Sending Login packet.---\n");
    send_packet(p_login);
    packet_free(p_login);
    dbg_print("---Login end.---\n\n");
    return NULL;
}

packet* mouse_report(packingfunc func, void* data)
{
    packet* p_report = packet_allocate();
    dbg_print("---Preparing Report packet.---\n");
    p_report->message_type = REPORT;
    p_report = func(p_report, data);
    dbg_print("---Sending Report packet.---\n");
    send_packet(p_report);
    packet_free(p_report);
    dbg_print("---Report end.---\n\n");
    return NULL;
}

packet* mouse_control()
{
    packet* p_control;
    return NULL;
}

packet* mouse_logout()
{
    packet* p_logout = packet_allocate();
    dbg_print("---Preparing Logout packet.---\n");
    p_logout->message_type = LOGOUT;
    dbg_print("---Sending Logout packet.---\n");
    send_packet(p_logout);
    packet_free(p_logout);
    dbg_print("---Logout end.---\n\n");
    return NULL;
}

void _print_head(packet* p)
{
    dbg_print("Message type: ");
    switch (p->message_type) {
    case ACK:
        dbg_print("ACK\n");
        break;
    case NACK:
        dbg_print("NACK\n");
        break;
    case LOGIN:
        dbg_print("LOGIN\n");
        break;
    case REPORT:
        dbg_print("REPORT\n");
        break;
    case CONTROL:
        dbg_print("CONTROL\n");
        break;
    case LOGOUT:
        dbg_print("LOGOUT\n");
        break;
    }
}

void _print_payload(packet* p)
{
    int i = 0, length = p->content_length;
    unsigned char* sptr = p->payload;

    dbg_print("Payload content(%d bytes):\n", length);
    while (i < length) {
        dbg_print("%02X ", sptr[i++]);
        if (i % 8 == 0)
            dbg_print("\n");
    }
    /* display 8 bytes per line. */
    if (length % 8 != 0)
        dbg_print("\n");
}

int send_packet(packet* p)
{
    unsigned char* buffer;
    buffer = (unsigned char*)malloc(1 + p->content_length);
    if (buffer == NULL) {
        fprintf(stderr, "send_packet():buffer allocate error!\n");
        return -1;
    }

    memcpy(buffer, &(p->message_type), 1);
    memcpy(buffer + 1, p->payload, p->content_length);

    _print_head(p);
    _print_payload(p);
    send(sockfd, buffer, p->content_length + 1, 0);
    dbg_print("%d byte has been sent.\n", p->content_length + 1);

    free(buffer);
    return 0;
}

packet* packet_allocate()
{
    packet* p;
    p = (packet*)malloc(sizeof(packet));
    if (p == NULL) {
        fprintf(stderr, "packet_allocate():packet allocate error!\n");
        return NULL;
    }
    p->size = PAYLOAD_INITALLOC_SIZE;
    p->content_length = 0;
    p->payload = (unsigned char*)malloc(p->size);
    //TODO: check NULL here
    return p;
}

void packet_free(packet* p)
{
    if (p != NULL) {
        free(p->payload);
        free(p);
    }
}

void packet_put_int(packet* p, int n)
{
    dbg_print("packet_put_int():%d\n", n);
    packet_put_buffer(p, (unsigned char*)&n, sizeof(n));
}

void packet_put_float(packet* p, float n)
{
    dbg_print("packet_put_float():%f\n", n);
    packet_put_buffer(p, (unsigned char*)&n, sizeof(n));
}

void packet_put_double(packet* p, double n)
{
    dbg_print("packet_put_double():%lf\n", n);
    packet_put_buffer(p, (unsigned char*)&n, sizeof(n));
}

void packet_put_char(packet* p, char n)
{
    dbg_print("packet_put_char():%c\n", n);
    packet_put_buffer(p, (unsigned char*)&n, sizeof(n));
}

void packet_put_buffer(packet* p, unsigned char* buffer, int length)
{
    int re_alloc = 0;
    while (p->size < p->content_length + length) {
        p->size *= 2;
        re_alloc = 1;
    }
    if (re_alloc) {
#if 0
        unsigned char* buffer; /* rename! */
        buffer = (unsigned char*)malloc(p->size);
        memcpy(buffer, p->payload, p->content_length);
        free(p->payload);
        p->payload = buffer;
#endif
        unsigned char* tmp_buffer;
        tmp_buffer = (unsigned char*)realloc(p->payload, p->size);
        if (tmp_buffer != NULL)
            p->payload = tmp_buffer;
        else {
            fprintf(stderr, "packet_put_buffer():packet re-allocate error!\n");
            return; //TOFIX: ?, what about packet_allocate?
        }
    }
    int i;
    for (i = 0; i < length; i++) {
        p->payload[p->content_length] = buffer[i];
        dbg_print("%d:%d\n", i, buffer[i]);
        p->content_length++;
    }
}
