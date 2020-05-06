/*  main.c  - main */

#include <xinu.h>

volatile int debugmem = 0;

process	main(void)
{
    
    int i = 0, size = 8;
    debugmem = 1;
    for(i = 0; i < 20; i++) {
        if(myfreemem(mygetmem(size), size) == SYSERR) {
            uprintf("Failed to allocate / free memory \n");
        }
    }

	return OK;    
}
