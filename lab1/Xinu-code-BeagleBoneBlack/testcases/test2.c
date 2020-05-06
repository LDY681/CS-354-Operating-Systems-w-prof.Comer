/*  test2.c  - main */

#include <xinu.h>

bool8 go = FALSE;

process main(void)
{
	resume(create(print, 256, 10, "Print A", 1, 'A'));
	resume(create(print, 256, 10, "Print B", 1, 'B'));
	resume(create(print, 256, 10, "Print C", 1, 'C'));

	go = TRUE;
}