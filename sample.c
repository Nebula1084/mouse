#include "mouse.h"
#include <stdio.h>

#define MOUSE_HOST "localhost"
#define MOUSE_PORT 10659

typedef struct testdata {
    float a;
    double b;
} testdata;

packet* pack_two_float(packet* p, void* data)
{
    testdata* dataptr = (testdata*)data;
    packet_put_int(p, 10);//device id
    packet_put_float(p, dataptr->a);
    packet_put_double(p, dataptr->b);
    return p;
}

int main()
{
    printf("hello!\n");
    mouse_init(0x12345678, MOUSE_HOST, MOUSE_PORT);
    mouse_login("abcdefghijklmnopqrstuvwxyzabcdef");

    testdata data = { 0.4f, 0.005 };
    mouse_report(pack_two_float, (void*)&data);
    
    mouse_logout();
    return 0;
}
