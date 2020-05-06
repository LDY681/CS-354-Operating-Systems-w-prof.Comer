/* myfreemem.c - myfreemem */
#include <xinu.h>
/*------------------------------------------------------------------------
 *  myfreemem  -  caches recently freed blocks so they can be quickly allocated again
 *------------------------------------------------------------------------
 */
syscall	myfreemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct blkList *head;
	struct memEntry mementry;
	struct blkList *block;
	mask = disable();
	
	if (nbytes == 0) {
		restore(mask);
		return SYSERR;
	}	

	//insert memblock to list
	int i;
	int lastNULL = 0;
	for (i = 0; i < 20; i ++){	
	
		//if size is matched, add free memblock to the head of the list
		if (arrStruct[i].blockSize == nbytes){	
			kprintf("free:if size is matched, add free memblock to the head of the list\n");
			mementry = (struct memEntry)arrStruct[i];
			head = (struct blkList *)mementry.list;
			block->next = head;	//make previous head as next
			block->blockaddr = blkaddr;
			mementry.list = block;	//set the head of that list
			restore(mask);
			return OK;
		}

		//if array entry is size 0, update lastNULL
		if (arrStruct[i].blockSize == 0){
			lastNULL = i;
		}
		
		//if size is not matched with any and array has been iterated completely
		if (i == 19){
		
			if (lastNULL != 0){		//if array has empty entry
			kprintf("free:if size is not matched with any and lastNULL != 0\n");
				//Initilize and make (struct blkList *)blkaddr as head
				mementry.blockSize = nbytes;
				head->blockaddr = blkaddr;
				head->next = NULL;
				mementry.list = head;
				arrStruct[lastNULL] = mementry;
				restore(mask);
				return OK;
			}else{	
				/*TODO: If the cache is full (all 20 entries are used), and a process frees a block of a new size, 
				simply invoke the original freemem to return the block to the free list.
				*/
				kprintf("free:calling freemem\n");
				freemem(blkaddr,nbytes);
				restore(mask);
				return OK;
			}
		}
	}
	restore(mask);
	return OK;
}
