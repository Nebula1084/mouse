#include "mouse.h"
#include <stdio.h>
#include <string.h>

#define MOUSE_HOST "localhost"
#define MOUSE_PORT 10659

#define KEY_FOR_REPORT1 "218c4326d373fc8d0e4b9db529330661"

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
    mouse_init(1, MOUSE_HOST, MOUSE_PORT);

    if (mouse_login(KEY_FOR_REPORT1))
    {
        reportData data = { 1, 4, 0.05f, 5678, "abcd1234"};
        mouse_report(pack_report, (void *)&data);
        mouse_logout();
    }
    return 0;
}
