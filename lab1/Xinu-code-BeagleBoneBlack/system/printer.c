/* printer.c - type, kerneltype, print, kernelprint */

#include <xinu.h>

/*------------------------------------------------------------------------
 * type - endlessly putc at a time
 *------------------------------------------------------------------------
 */
syscall type(
	  char c			/* character to write 		*/
	)
{
		while (go == FALSE){}
		while (1){
		  putc(stdout,c);
		}
}

/*------------------------------------------------------------------------
 * kerneltype - endlessly kputc at a time
 *------------------------------------------------------------------------
 */
syscall kerneltype(
	  char c			/* character to write 		*/
	)
{
	while (go == FALSE){}
	while (1){
	  kputc(c);
	}
}

/*------------------------------------------------------------------------
 * print - endlessly uprintf strings of 50 characters
 *------------------------------------------------------------------------
 */
syscall print(
	  char c			/* character to form array from */
	)
{
	while (go == FALSE){}
	char arr[51];
	int i = 0;
	for (i = 0; i < 50; i++){
	  arr[i] = c;
	}
	  arr[50] = '\0';	
	while (1){
	  uprintf(arr);
	}
}

/*------------------------------------------------------------------------
 * kernelprint - endlessly kprintf strings of 50 characters
 *------------------------------------------------------------------------
 */
syscall kernelprint(
	  char c			/* character to form array from */
	)
{
	while (go == FALSE){}
	char arr[51];
	int i = 0;
	for (i = 0; i < 50; i++){
	  arr[i] = c;
	}
	  arr[50] = '\0';	
	while (1){
	  kprintf(arr);
	}
}
