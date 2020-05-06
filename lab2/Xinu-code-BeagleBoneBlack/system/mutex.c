/* mutex.c - mutex_init */

#include <xinu.h>
sid32 mutex = NULL;
int32 sum = 0;
pid32 lastpid = 0;
void mutex_init(
        void
    )
{
    // TODO: initialize the mutex here
	mutex = semcreate(1);
}

void incrementer(
	uint32 val, 
	uint32 iter
	)
{
	int i;
	for (i = 0; i < iter; i++){
		wait(mutex);
		sum += val;
		lastpid = currpid;
		signal(mutex);
		sleepms(50);
	}
}

void monitor(
		void
	){
	while (1){
		sleepms(800);
		kprintf("Sum is %d, lastpid is %d\n", sum, lastpid);
	}
}