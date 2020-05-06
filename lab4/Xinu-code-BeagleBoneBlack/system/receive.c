/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	//struct	procent *blksender;		/* process table entry of blocked sender*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	if (prptr->hascb == TRUE){
		receivecallback(prptr->cb(msg));
	}
	//unblock sending process if message was sent by sendblk
	if (prptr->hasblockedsender == TRUE){
		prptr->hasblockedsender = FALSE;
		//blksender = &proctab[prptr->blockedsender];
		ready(prptr->blockedsender);
	}
	
	
	restore(mask);
	return msg;
}

/*------------------------------------------------------------------------
 *  receivecallback - save a callback function to be used by senders
 *------------------------------------------------------------------------
 */
syscall	receivecallback(
		callback c
	)
{
	return OK;
}
