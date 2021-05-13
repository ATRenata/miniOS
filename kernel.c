#include<stdlib.h>
#include<stdio.h>

#include "pcb.h"
#include "shell.h"
#include "ram.h"
#include "cpu.h"


int myinit(FILE *p) {
	PCB *pcb;
	int result;
	// printf("This is the myinit p file pointer %p\n", p );

	result = addToRAM(p);
	// printf("This is before adding to RAM\n");
	if (result>=0) {
		pcb = makePCB(p,result);
		// printf("%d\n", pcb->pages_max);
		// printf("this is the file pointer %p\n", pcb->PC);
		if (pcb != NULL) {
			// printf("The pointer File address %p\n", pcb->PC);
			// printf("This is before adding to reay queue\n");
			addToReady(pcb);
			// printf("This is after adding pcb to rq\n");
			return 1;
		}
	}

	return 0;
}

void terminate(PCB *p) {
	clearRAM(p->start);
	free(p);
}

void scheduler() {
	PCB *pcb;
	int result;

	// initialize CPU
	initCPU();

	// execute the processes
	while(getHead() != NULL) {
		// printPCB(); // for debugging

		pcb = getFromReady();

		if (pcb != NULL) {
			setCPU(pcb->PC, pcb->PC_offset);
			// printf("PCb  %p\n", pcb->PC);
			result = runCPU(2, pcb);

			// printf("Runned CPU succesfully \n");
			if (result == 99) terminate(pcb);
			else addToReady(pcb);
		} 	
	}

	// delete the file in the BackingStore
	system("rm -r -f BackingStore/*");


}


void boot(){
	initRAM();
	system("rm -f -r BackingStore");
	system("mkdir BackingStore");	
}
int main() {
	boot();
	int result = 0;

	initRAM();
	initCPU();

	result = shell();

	return result;
}
