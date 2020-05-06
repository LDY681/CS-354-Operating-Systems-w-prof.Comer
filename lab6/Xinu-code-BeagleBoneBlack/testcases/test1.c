/*  test1.c  - main */

#include <xinu.h>

process	main(void)
{

	resume(create(kerneltype, 256, INITPRIO, "Print A", 1, 'A'));
	resume(create(kerneltype, 256, INITPRIO, "Print B", 1, 'B'));
	resume(create(kerneltype, 256, INITPRIO, "Print C", 1, 'C'));
    
}
