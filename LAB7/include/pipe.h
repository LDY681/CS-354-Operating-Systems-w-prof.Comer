/* pipe.h */

/*Size for pipe table*/
#ifndef NPIPE
#define	NPIPE		10
#endif	

/* Pipe status constants */
#ifndef	PIPE_FREE
#define	PIPE_FREE 0		/* pipe not allocated	*/
#endif
#ifndef	PIPE_READY
#define	PIPE_READY	1		/* pipe allocated but not used yet	*/
#endif
#ifndef	PIPE_CURR
#define	PIPE_CURR	2		/* pipe in used by sending and calling process	*/
#endif

/* Pipe property constants*/
#ifndef	PIPE_LEN
#define	PIPE_LEN 1024		/* size of pipe buffer	*/
#endif

/* Definition of the pipe table */

struct pipeent {		/* Entry in the pipe table		*/
	uint16	pipestate;	/* pipe_free, etc */
	pid32	sender;		/* the process that send chars to pipe */
	pid32 	receiver; 	/* the process that receive chars from pipe */
	sid32	pipeisem; 		/* producer semaphore, don't produce when full */
	sid32 	pipeosem;		/* consumer semaphore, don't consume when empty */
	int32 	pipebuff[PIPE_LEN]; 	/*The buffer of pipe, type int32 to cinlude EOF*/
	int32 *pipehead;	/* Next input char to read */
	int32 *pipetail; 	/* Next slot for arriving char */
};

extern	struct	pipent pipetab[];	/* pipe table*/

#define PIPE_EOF ’\0’ /* The EOF indicator sender sends */