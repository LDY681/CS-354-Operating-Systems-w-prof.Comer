/*  test2.c  - test2 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * 2 producers producing 15 characters in total, and 2 consumers consuming 15 characters in total.
 * No leftover expected.
 *------------------------------------------------------------------------
 */
process	main(void)
{
	produce_consume_init();

	create_producer('A', 10);
	create_producer('B', 5);
	create_consumer(5);
	create_consumer(10);

	sleep(3);

	produce_consume_destroy();

	return OK;
}