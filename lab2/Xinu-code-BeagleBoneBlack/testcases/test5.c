/*  test5.c  - test5 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * Three processes, A adds 1 to the sum 200 times, B adds 10 200 times, and C adds 100 200 times.
 * Expected sum: 22200 at the end.
 *------------------------------------------------------------------------
 */
process	main(void)
{
    mutex_init();
    
	resume(create(monitor, 512, 100, "Monitor Process", 0, NULL));
	resume(create(incrementer, 512, 10, "Incrementer A", 2, 1, 200));
	resume(create(incrementer, 512, 10, "Incrementer B", 2, 10, 200));
    resume(create(incrementer, 512, 10, "Incrementer C", 2, 100, 200));

	return OK;
}