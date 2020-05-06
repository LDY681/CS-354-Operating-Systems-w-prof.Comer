/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	mymeminit();
	uint32 i, size = 8;
	
	for(i = 0; i < 20; i++) {
		if(myfreemem(mygetmem(size), size) == SYSERR) {
			uprintf("Failed to allocate / free memory.\n");
		}
	}

	return OK;
    
}
