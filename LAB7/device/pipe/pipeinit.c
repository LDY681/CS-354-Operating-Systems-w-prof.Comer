/* pipeinit.c - pipeinit */

#include <xinu.h>

struct	pipent	pipetab[NPIPE];
/*------------------------------------------------------------------------
 *  pipeinit  -  Initialize pipe data structure and create semaphores
 *------------------------------------------------------------------------
 */
devcall pipeinit(void)
{
	
	intmask mask;				/* interrupt mask				*/
	pipid32 pipe;				/* pipe id*/

	mask = disable();			/* disable interrupts at start of function	*/
	
	/*Initialize pipetable*/
	struct	pipeent	*pipeptr;	/* pipe pointer*/
	for (i = 0; i < NPIPE; i++) {
		pipeptr = &pipetab[i];
		memset(pipeptr->pipebuff, '\0', PIPE_LEN);
		pipeptr->pipstate = PIPE_FREE;		
		pipeptr->reader = -1;
		pipeptr->writer = -1;
		pipeptr->pipeisem = semcreate(1024);
		pipeptr->pipeosem = semcreate(0);
		pipeptr->head = 0;
		pipeptr->tail = 0;
	}
	kprintf("Pipe table initialized\n");
	restore(mask);				/* restore interrupts before normal return	*/
}
