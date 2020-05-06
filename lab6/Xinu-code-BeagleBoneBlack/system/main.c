/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>     /* strtoul */
process	main(void)
{
	all();
	recvclr();
	kprintf("Now we run the Xinu Shell\n");
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}

syscall all(){
	printf("Type 1/2 and hit enter to run part 1/2: ");
	char question;
	question = fgetc(stdin);
	fgetc(stdin);	//get rid of "\n"
	if (question == '1'){
		part1();
		all();
	}else if (question == '2'){
		part2();
	}else{
		kprintf("question can be only 1 or 2\n");
		all();
	}
	return OK;
}

syscall part1(){
	kprintf("Running part1\n");
	
	//Specify remote IP
	char *ipStr = "128.10.137.64";
	uint32 remip = 0;
	//kprintf("starting dot2ip\n");
	dot2ip(ipStr, &remip);
	//kprintf("remip is: %d\n",remip);
	//Specify remote port
	uint16 remport = 44000;
	
	//TODO: Specify local port (need assignment from TA)
	uint16 locport = 55315;
	
	//register a slot if not yet
	uid32 slot;
	if ((slot = udp_register(remip, remport, locport)) == SYSERR){
		kprintf("cannot register the slot\n");
		return SYSERR;
	}
	//kprintf("slot is: %d\n", slot);
	
	//udp_send
	char sendbuff[] = "You Can't Judge a Book By Its Cover681681681681681";
	int32 len = strlen(sendbuff);
	udp_send(slot,sendbuff,len);
	
	char recvbuff[1024];
	//udp_recv, try twice
	if (udp_recv(slot,recvbuff,1024,500) == TIMEOUT){
		if (udp_recv(slot,recvbuff,1024,500) == TIMEOUT){
			kprintf("failed");
			return SYSERR;
		}
	}
	//kprintf("%d %d %s %s\n",strlen(sendbuff),strlen(recvbuff),sendbuff,recvbuff);
	
	//check if contents and size is the same
	if(strlen(sendbuff) == strlen(recvbuff)){
		int i;
		for (i = 0; i < strlen(sendbuff); i++){
			if (sendbuff[i] != recvbuff[i]){
				kprintf("failed");
				return SYSERR;
			}
		}
		kprintf("success\n");
		
	}
	udp_release(slot);	//release the registered slot
	memset(recvbuff, 0, sizeof(recvbuff));	//empty the buffer;
	return OK;
}

syscall part2(){
	kprintf("Running part2\n");	
	uint16 smallerport = 55315;	
	uint16 largerport = 55316;	
	
	//Get identity sender or receiver 
	printf("Type s/r to indicate sender/receiver: ");
	char identity;
	identity = fgetc(stdin);
	fgetc(stdin);	//get rid of "\n"
	
	//print local ip address
	kprintf("Local IP of this machine is:\n");
	uint32 locip = getlocalip(); 
	//kprintf("%u\n",locip);
	kprintf("%u.%u.%u.%u\n",locip >> 24,(locip >> 16) & 0xFF,(locip >> 8) & 0xFF,locip & 0xFF);

	//Ask to input destination ip and convert to uint
	printf("Please provide the IP address of the other computer\n");
	char ipStr[1024]; 
    fgets(ipStr, 1024, stdin); 
    if (ipStr[strlen(ipStr) -1] == '\n'){
		ipStr[strlen(ipStr) -1] = '\0';
	} 
	uint32 remip;
	dot2ip(ipStr, &remip);
	//kprintf("destination Ip is: %u\n", remip);
	
	//remote port
	uint16 remport = 0;
	uint16 locport = 0;
	
	//if locip < remip, local port is smaller one, so remote port is larger one
	if (locip < remip){
		locport = smallerport;
		remport = largerport;
	}else{
		locport = largerport;
		remport = smallerport;
	}

	//kprintf("%u %u %u %c\n",remip,remport,locport, identity);
	//register a slot if not yet
	uid32 slot;
	if ((slot = udp_register(remip, remport, locport)) == SYSERR){
		kprintf("cannot register the slot\n");
		return SYSERR;
	}
	
	//udp_send if identity is sender
	if (identity == 's'){
		char sendbuff[1024]; 
		bool8 prevblank = FALSE;
		bool8 readytokill = FALSE;
		while (readytokill == FALSE){
			printf("Enter your message:");
			fgets(sendbuff, 1024, stdin); 
			
			//check if two successive blank strings
			if (sendbuff[0] == '\n'){
				if (prevblank == TRUE){	//if prev string is already blank
					readytokill = TRUE;
				}else{
					prevblank = TRUE;	//if current string is the first blank string
				}
			}else{
				prevblank = FALSE;	//set prev blank to false
			}
			
			//regular send
			if (sendbuff[strlen(sendbuff) -1] == '\n'){
				sendbuff[strlen(sendbuff) -1] = '\0';
			} 
			int32 len = strlen(sendbuff);
			udp_send(slot,sendbuff,len);
		}
		udp_release(slot);	//release the registered slot
		printf("sending process killed\n");
		return OK;
	}else if (identity == 'r'){
		char recvbuff[1024];
		bool8 prevblank = FALSE;
		bool8 readytokill = FALSE;
		while (readytokill == FALSE){
			//udp_recv, try twice
			if (udp_recv(slot,recvbuff,1024,500) != TIMEOUT){
				kprintf("%s\n",recvbuff);
				
				//check if two successive blank strings
				if (recvbuff[0] == '\0'){
					if (prevblank == TRUE){	//if prev string is already blank
						readytokill = TRUE;
					}else{
						prevblank = TRUE;	//if current string is the first blank string
					}
				}else{
					prevblank = FALSE;	//set prev blank to false
				}
			
				memset(recvbuff, 0, sizeof(recvbuff));	//empty the buffer;
			}
		}
		udp_release(slot);	//release the registered slot
		printf("receiving process killed\n");
		return OK;
	}else{
		kprintf("identity has to be either s(sender) or r(receiver)\n");
	}	
	return OK;
}

