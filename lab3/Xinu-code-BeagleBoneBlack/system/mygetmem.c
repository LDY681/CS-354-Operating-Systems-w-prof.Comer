/* mygetmem.c - mygetmem */
#include <xinu.h>
/*------------------------------------------------------------------------
 *  mygetmem  -  search the cache and return a free block
 *------------------------------------------------------------------------
 */

char *mygetmem(
		uint32 nbytes
	){
	
	intmask	mask;			/* Saved interrupt mask		*/
	struct blkList *head;
	struct memEntry mementry;
	struct blkList *next;

	mask = disable();
	
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	int i;
    for (i = 0; i < 20; i ++){	
		//if size is matched, get the first memblock of that list
		if (arrStruct[i].blockSize == nbytes){	
			kprintf("get:if size is matched, get the first memblock of that list\n");
			mementry = arrStruct[i];
			//kprintf("bug1\n");
			head = mementry.list;
			//kprintf("bug2\n");
			//kprintf("%c\n",head == NULL);
			//kprintf("%c\n",head->next == NULL );
			//modify blklist
			if (head != NULL && head->next != NULL){	//if there are more than one block in list
				kprintf("get:head!=next\n");
				next = head->next;
				mementry.list = next;	//set head to next
				
			}else{	//last block(head->next == head), make blklist reuseable (size 0)
				kprintf("get:make list reuseable\n");
				struct memEntry new;
				new.blockSize =  0;
				arrStruct[i] = new;
			}
		
			//return the address of the free block
			restore(mask);
			return (char *)head;
		}
		
		//if size is not matched with any and array has been iterated completely
		if (i == 19){
			kprintf("get:if size is not matched with any and array has been iterated completely\n");
			//If the cache fails to return a free block, the free list should be searched using the original getmem
			restore(mask);
			return (char *)getmem(nbytes);
		}
		
	}
	
	restore(mask);
	return (char *)SYSERR;
}