/* limit.c - tlimit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * tlimit - set time slice limit for process of a given pid
 *------------------------------------------------------------------------
 */
syscall tlimit(
	  pid32 id,		/* the process id */
	  int16 limit	/* the number of additional time slices a process can run */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	
	//limit has to be positive
	if (limit > 0){
		//if id is current process id 
		if (proctab[(id)].prstate == PR_CURR){
			int16 newlimit = limit+1;
			proctab[(id)].nslices = newlimit;
		}else{
			proctab[(id)].nslices = limit;
		}
	}else{
		restore(mask);
		return SYSERR;
	}
	/*char buf[256];
	sprintf (buf, "proctab[(%d)].nslices = %d\n",id,proctab[(id)].nslices);
	uprintf(buf);
	*/
	restore(mask);
	return OK;
}
