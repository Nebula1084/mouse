#include "mouse.h"
#include <stdio.h>
#include <string.h>

// #define MOUSE_HOST "localhost"
#define MOUSE_HOST "106.2.108.194"
#define MOUSE_PORT 10659

//md5(md5(device_id)+device_name)
#define KEY_FOR_LOGIN "a04e820a8c636ba1eb121f6463d418ff"

typedef struct reportData {
    int device_id;
    int report_id;
    float field0;
    int field1;
    unsigned char* field2;
} reportData;

packet* pack_report(packet* p, void* data)
{
    reportData* dataptr = (reportData*)data;

    packet_put_int(p, dataptr->device_id);
    packet_put_int(p, dataptr->report_id);
    packet_put_float(p, dataptr->field0);
    packet_put_int(p, dataptr->field1);
    
    packet_put_buffer(p, dataptr->field2, 8);
    return p;
}

int main()
{
    printf("hello!\n");

    packet* pptr = NULL;
    mouse_init(3, MOUSE_HOST, MOUSE_PORT);

    if (mouse_login(KEY_FOR_LOGIN))
    {
        reportData data = { 3, 4, 0.001f, 12345678, "ABCDEFGH"};
        mouse_report(pack_report, (void *)&data);

        //pptr = mouse_control_recv(1, 1);

        mouse_logout();
    }
    return 0;
}
