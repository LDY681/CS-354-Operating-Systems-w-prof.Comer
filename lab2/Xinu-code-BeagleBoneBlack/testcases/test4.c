/*  test4.c  - test4 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * Two processes, A adds 1 to the sum 100 times, and B adds 3 to the sum 80 times.
 * Expected sum: 340 at the end.
 *------------------------------------------------------------------------
 */
process	main(void)
{
	mutex_init();
	
	resume(create(monitor, 512, 100, "Monitor Process", 0, NULL));
	resume(create(incrementer, 512, 10, "Incrementer A", 2, 1, 100));
	resume(create(incrementer, 512, 10, "Incrementer B", 2, 3, 80));

	return OK;
}