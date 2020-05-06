/*  test3.c  - test3 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * 2 producers producing 20 characters in total, and 2 consumers consuming 12 characters in total.
 * 8 leftovers expected.
 *------------------------------------------------------------------------
 */
process	main(void)
{
	produce_consume_init();

	create_producer('A', 10);
	create_producer('B', 10);
	create_consumer(8);
	create_consumer(4);

	sleep(3);

	produce_consume_destroy();

	return OK;
}