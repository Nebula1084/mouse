#include "mouse.h"
#include <stdio.h>

#define MOUSE_HOST "10.110.33.227"
#define MOUSE_PORT 10659

int main(){
    printf("hello!\n");
	mouse_init(10, MOUSE_HOST,MOUSE_PORT);
	mouse_login("abcdefghijklmnoprstuvwxyzabcdef");
}
