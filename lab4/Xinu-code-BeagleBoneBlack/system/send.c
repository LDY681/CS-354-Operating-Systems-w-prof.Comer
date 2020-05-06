/* send.c - send, sendblk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {	/* Receive with timeout */
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*------------------------------------------------------------------------
 *  sendblk  -  Pass a message to a process and block until it is received
 *------------------------------------------------------------------------
 */
syscall	sendblk(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {	//if a process already has a message, sendblk returns SYSERR (just like send), otherwise return OK
		restore(mask);
		return SYSERR;
	}
	
	prptr->prmsg = msg;		/* sendblk must store the message (just like send) */
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	if (prptr->prstate == PR_RECV) {	//if the receiving process is in the PR_RECV state, blocked awaiting a message
		/* sendblk makes the receiving process ready (just like send)*/
		ready(pid);
	} else {	
		//Set a bit to indicate that a sending process is blocked
		prptr->hasblockedsender = TRUE;
		//and must also record the process ID of the sending process in the recipient's process table. 
		prptr->blockedsender = currpid;
		//make the sender change itself to a new state, PR_SENDING, 
		struct	procent *currptr = &proctab[currpid];
		currptr->prstate = PR_SENDING;
		//and reschedule to allow other processes to run. 
		resched();
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*------------------------------------------------------------------------
 *  sendcallback  -  Pass a message to a process using its callback function
 *------------------------------------------------------------------------
 */
syscall	sendcb(
		pid32	pid,		/* ID of recipient process	*/
		umsg32	msg,		/* Contents of message		*/
		uint32 (* cb) ()	/* Callback pointer			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {	//if a process already has a message, sendblk returns SYSERR (just like send), otherwise return OK
		restore(mask);
		return SYSERR;
	}
	
	prptr->prmsg = msg;		/* sendblk must store the message (just like send) */
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/
	prptr->hascb = TRUE;
	prptr->cb = cb;
	if (prptr->prstate == PR_RECV) {	//if the receiving process is in the PR_RECV state, blocked awaiting a message
		/* sendblk makes the receiving process ready (just like send)*/
		ready(pid);
	} else {	
		//Set a bit to indicate that a sending process is blocked
		prptr->hasblockedsender = TRUE;
		//and must also record the process ID of the sending process in the recipient's process table. 
		prptr->blockedsender = currpid;
		//make the sender change itself to a new state, PR_SENDING, 
		struct	procent *currptr = &proctab[currpid];
		currptr->prstate = PR_SENDING;
		//and reschedule to allow other processes to run. 
		resched();
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
