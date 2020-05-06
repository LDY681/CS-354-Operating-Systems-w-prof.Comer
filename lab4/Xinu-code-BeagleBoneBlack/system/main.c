/*  main.c  - main */

#include <xinu.h>

#define RED		"\e[31m"
#define YELLOW	"\e[93m"
#define GREEN	"\e[32m"
#define RESET	"\e[0m"

pid32 recvpid;
pid32 sendpid;

status test4_retcode = OK;
bool8 test5_passed = FALSE;

process receiver_1() {
	struct procent *proc = &proctab[currpid];
	recvpid = currpid;
	receive();
	umsg32 msg = proc->prmsg;
	
	if(msg == 0xdeadbeef) {
		kprintf("%s\tTest passed!%s\n", GREEN, RESET);
		return OK;
	}
	kprintf("%s\tTest failed!%s\n", RED, RESET);
	return SYSERR;
}

process sender_1() {
	sendblk(recvpid, 0xdeadbeef);
	return OK;
}

/*	Test 1 tests the base case of send() works within sendblk() */

process test_1() {
	kprintf("%s===== Beginning test 1 (sendblk) =====%s\n", YELLOW, RESET);
	resume(create(receiver_1, 8192, 20, "receiver 1", 0, NULL));
	resume(create(sender_1, 8192, 10, "sender 1", 0, NULL));
	return OK;
}

process receiver_2() {
	struct procent *proc = &proctab[currpid];
	recvpid = currpid;
	sleep(1);

	struct procent *sender = &proctab[proc->blockedsender];
	if(! proc->hasblockedsender || sender->prstate != PR_SENDING) {
		kprintf("%s\tTest failed!%s\n", RED, RESET);
		return SYSERR;
	}

	receive();
	umsg32 msg = proc->prmsg;
	
	if(msg == 0x12345678) {
		kprintf("%s\tTest passed!%s\n", GREEN, RESET);
		return OK;
	}
	kprintf("%s\tTest failed!%s\n", RED, RESET);
	return SYSERR;
}

process sender_2() {
	sendpid = currpid;
	sendblk(recvpid, 0x12345678);
	return OK;
}

/*	Test 2 tests the case where sendblk() sends to a sleeping process
 *	Additionally tests that flags are set correctly within sending and
 *	receiving processes
 */

process test_2() {
	kprintf("%s===== Beginning test 2 (sendblk) =====%s\n", YELLOW, RESET);
	resume(create(receiver_2, 8192, 20, "receiver 2", 0, NULL));
	resume(create(sender_2, 8192, 10, "sender 2", 0, NULL));
	return OK;
}

process receiver_3() {
	struct procent *proc = &proctab[currpid];
	recvpid = currpid;

	receive();
	umsg32 msg = proc->prmsg;
	if(msg != 0x87654321) {
		kprintf("%s\t1st receive msg check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	receive();
	msg = proc->prmsg;
	if(msg != 0x90abcdef) {
		kprintf("%s\t2nd receive msg check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	receive();
	msg = proc->prmsg;
	if(msg != 0x99990000) {
		kprintf("%s\t3rd receive msg check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	sleep(1);
	struct procent *sender = &proctab[proc->blockedsender];
	if(! proc->hasblockedsender || sender->prstate != PR_SENDING ||
		proc->prmsg != 0x25262728) {
			kprintf("%s\tput to sleep hasblockedsender, senderstate and msg test: Test failed!%s\n", RED, RESET);
			return SYSERR;
	}

	receive();
	if(proc->hasblockedsender || sender->prstate != PR_READY) {
		kprintf("%s\t4rd receive hasblockedsender, senderstate check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	msg = proc->prmsg;
	if(msg != 0x25262728) {
		kprintf("%s\t4rd receive msg check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	sleepms(500);
	recvclr();
	msg = proc->prmsg;
	if(msg != 0xbeefcafe) {
		kprintf("%s\tsleep+recvclr msg check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	if(proc->hasblockedsender || sender->prstate != PR_READY) {
		kprintf("%s\tsleep+recvclr hasblockedsender, senderstate check: Test failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	kprintf("%s\tTest passed!%s\n", GREEN, RESET);
	return OK;
}

process sender_3() {
	sendpid = currpid;
	sendblk(recvpid, 0x87654321);
	sendblk(recvpid, 0x90abcdef);
	sendblk(recvpid, 0x99990000);
	sendblk(recvpid, 0x25262728);
	sendblk(recvpid, 0xbeefcafe);
	return OK;
}

/*	Test 3 tests repeated calls to sendblk, one of which blocks
 *	Checks that process state is reset after a call to recvclr()
 */

process test_3() {
	kprintf("%s===== Beginning test 3 (sendblk) =====%s\n", YELLOW, RESET);
	resume(create(receiver_3, 8192, 20, "receiver 3", 0, NULL));
	resume(create(sender_3, 8192, 10, "sender 3", 0, NULL));
	return OK;
}

process receiver_4() {
	struct procent *proc = &proctab[currpid];
	recvpid = currpid;
	sleep(1);
	
	receive();
	umsg32 msg = proc->prmsg;
	if(msg != 0xdddddddd) {
		kprintf("%s\tTest failed!%s\n", RED, RESET);
		return SYSERR;
	}
	
	sleepms(500);
	if(test4_retcode != SYSERR)
		kprintf("%s\tTest failed!%s\n", RED, RESET);
	
	receive();
	msg = proc->prmsg;
	if(msg != 0xffffffff) {
		kprintf("%s\tTest failed!%s\n", RED, RESET);
		return SYSERR;
	}
	kprintf("%s\tTest passed!%s\n", GREEN, RESET);
	return OK;
}

process sender_4_2() {
	sendpid = currpid;
	sendblk(recvpid, 0xdddddddd);
	sendblk(recvpid, 0xffffffff);
	return OK;
}

process sender_4() {
	sendpid = currpid;
	resume(create(sender_4_2, 8192, 10, "sender 4.2", 0, NULL));
	test4_retcode = sendblk(recvpid, 0xeeeeeeee);
	return OK;
}

/*	Test 4 tests repeated calls to sendblk() with two sender processes,
 *	one of which fails
 */

process test_4() {
	kprintf("%s===== Beginning test 4 (sendblk) =====%s\n", YELLOW, RESET);
	resume(create(receiver_4, 8192, 20, "receiver 4", 0, NULL));
	resume(create(sender_4, 8192, 10, "sender 4", 0, NULL));
	return OK;
}

process receiver_5() {
	recvpid = currpid;
	receive();
	return OK;
}

uint32 printnum(uint32 num) {
	if(num == 0xabcd0000){
		kprintf("%s\tTest passed!%s\n", GREEN, RESET);
		test5_passed = TRUE;
		return num;
	}
	return SYSERR;
}

process sender_5() {
	sendpid = currpid;
	sendcb(recvpid, 0xabcd0000, printnum);
	sleep(1);
	if(! test5_passed)
		kprintf("%s\tTest failed!%s\n", RED, RESET);
	return OK;
}


/*	Test 5 tests part 2's sendcb() functionality */

process test_5() {
	kprintf("%s===== Beginning test 5 (sendcb) =====%s\n", YELLOW, RESET);
	resume(create(receiver_5, 8192, 20, "receiver 5", 0, NULL));
	resume(create(sender_5, 8192, 10, "sender 5", 0, NULL));
	return OK;
}

process	main(void)
{

	resume(create(test_1, 8192, 30, "Test 1", 0, NULL));
	sleep(1);
	resume(create(test_2, 8192, 30, "Test 2", 0, NULL));
	sleep(2);
	resume(create(test_3, 8192, 30, "Test 3", 0, NULL));
	sleep(2);
	resume(create(test_4, 8192, 30, "Test 4", 0, NULL));
	sleep(3);
	resume(create(test_5, 8192, 30, "Test 5", 0, NULL));

	return OK;
    
}
