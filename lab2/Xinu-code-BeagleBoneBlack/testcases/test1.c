/*  test1.c  - test1 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * 1 producer producing 5 characters, and 1 consumer consuming 5 characters.
 * No leftover expected.
 *------------------------------------------------------------------------
 */
process	main(void)
{
	produce_consume_init();

	create_producer('A', 5);
	create_consumer(5);

	sleep(3);

	produce_consume_destroy();

	return OK;
}