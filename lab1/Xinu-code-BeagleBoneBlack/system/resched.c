/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;
/* Definition for linking of global variable*/
int16 debugsched = 0;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	/* Modify Time Slice Limits, do nothing if slice is -1*/
	if (ptold->nslices >= 1){
		ptold->nslices -= 1;
		/*char buf[256];
		sprintf (buf, "\ncurrpid%d->nslice = %d ->stacksize =%d\n",currpid,ptold->nslices,ptold->prstklen);
		uprintf(buf);
		*/
	}
			
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */		
		if (ptold->nslices == 0){
			/*If debugsched is non-zero, print a debug message whenever you are about to terminate a process */
			if (debugsched != 0){
				char arr[50]="\nAbout to kill process\n";
				uprintf(arr);
			}  
			kill(currpid);
			return;
		}else{
			if (ptold->prprio > firstkey(readylist)) {
				return;
			}
			/* Old process will no longer remain current */
			
			ptold->prstate = PR_READY;
			insert(currpid, readylist, ptold->prprio);	
		}
	}

	/* Force context switch to highest priority ready process */
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
