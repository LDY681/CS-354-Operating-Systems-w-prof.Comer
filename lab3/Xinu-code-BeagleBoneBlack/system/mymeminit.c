/* mymeminit.c - mymeminit */

#include <xinu.h>
struct memEntry arrStruct[20];		// array of structures.

/*------------------------------------------------------------------------
 * mymeminit - initilize array of structures for free list
 *------------------------------------------------------------------------
 */
void mymeminit(void)
{
	/* Initialize the arrStruct */
	int i;
	for (i = 0; i < 20; i++){
		struct memEntry mementry;
		mementry.blockSize =  0;
		arrStruct[i] = mementry;
	}
	
    kprintf("mymeminit: initialization done. \n");
   	
}
