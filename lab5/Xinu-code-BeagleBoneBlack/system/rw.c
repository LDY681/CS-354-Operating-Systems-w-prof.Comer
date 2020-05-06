/* rw.c - read_lock, write_lock, create_lock */

#include <xinu.h>

/*------------------------------------------------------------------------
 * read_lock - takes a lock as a reader
 *------------------------------------------------------------------------
 */
void read_lock(
		uint8	lid
	)
{
	
}

/*------------------------------------------------------------------------
 * write_lock - takes a lock as a writer
 *------------------------------------------------------------------------
 */
void write_lock(
		uint8 lid
	)
{
	
}

/*------------------------------------------------------------------------
 * create_lock - initialize lock data structure
 *------------------------------------------------------------------------
 */
uint8 create_lock(
		void
	)
{
	//TODO: Initialize your custom lock structure. You should not need parameters, and testcases will call this function as-is and expect a lock id (lid) into index into locktab.
}
