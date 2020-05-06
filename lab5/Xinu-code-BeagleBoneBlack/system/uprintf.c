/* kprintf.c -  kputc, kgetc, kprintf */

#include <xinu.h>
#include <stdarg.h>

extern	void	_doprnt(char *, va_list, int (*)(int), int);

/*------------------------------------------------------------------------
 * uprintf - use polled I/O to print formatted test with interrupts disabled
 *------------------------------------------------------------------------
 */
syscall uprintf(char *fmt, ...)
{
		intmask mask;
		mask = disable();
		
    va_list ap;
    va_start(ap, fmt);
    _doprnt(fmt, ap, (int (*)(int))kputc, (int)&devtab[CONSOLE]);
    va_end(ap);

		restore(mask);
    return OK;
}
