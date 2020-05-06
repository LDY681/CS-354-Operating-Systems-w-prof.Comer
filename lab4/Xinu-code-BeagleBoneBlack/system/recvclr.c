/* recvclr.c - recvclr */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvclr  -  Clear incoming message, and return message if one waiting
 *------------------------------------------------------------------------
 */
umsg32	recvclr(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	//struct	procent *blksender;		/* process table entry of blocked sender*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == TRUE) {
		msg = prptr->prmsg;	/* Retrieve message		*/
		prptr->prhasmsg = FALSE;/* Reset message flag		*/
		} else {
		msg = OK;
	}
	
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
