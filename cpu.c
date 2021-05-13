#include <stdio.h>
#include <stdlib.h>

#include "ram.h"
#include "pcb.h"
#include "shell.h"
#include "memorymanager.h"

// Global data structures simulating hardware
//

struct CPU {
	FILE *IP;
	char IR[1000];
	int quanta;
	int offset;
} cpu;

void initCPU() {
	cpu.quanta = 2;
	cpu.IP     = NULL;
	cpu.IR[0]  = '\0';
	cpu.offset = 0;
}

void setCPU(FILE *PC, int pcb_offset) {
	cpu.IP    = PC;
	cpu.IR[0] = '\0';
	cpu.offset = pcb_offset;
}

int runCPU(int quanta, PCB *pcb) {
	int result;
	char *p;

	cpu.quanta = quanta;

	int pcbOffset  = pcb->PC_offset;
	int cpuOffset  = cpu.offset ;

	while(cpu.quanta > 0) {

		// printf("PC_OFFSET -  %d   ;  CPU_OFFSET  - %d\n", pcbOffset, cpuOffset);

		// handling page fault 
		if(pcbOffset == 4){
			int page = pcb->PC_page + 1;
	
			// page exceeds pcb mas
			if(page > pcb->pages_max){
				clearRAM(pcb->start);
				free(p); 
			}
			else//otherwise we check to see if the frame for that page exists in the pageTable array.
			{ 
				for(int i = 0; i < 10; i++){
					if(pcb->pageTable[i] == page){
						//reset offset to 0
						pcb->PC_offset = 0;

						pcb->PC = ram[page];
					}
				}
				
				//Here, our pageTable does not contain the frame
				if(pcb->PC_offset != 0){
					
					FILE * f = findPage(page, pcb->PC); 

					//find space in RAM 
					int space = findFrame(f);
					int victim;
				
					if(space == -1){
						victim = findVictim(pcb);
					}

					// update the page tables
					updatePageTable(pcb, page, space, victim);

					// update the ram 
					if(space >= 0){
						ram[space] = f;
						pcb->PC=ram[space];
					}else{
						ram[victim] = f;
						pcb->PC=ram[victim];
					}
					pcb->PC_page = page;
					pcb->PC_offset = 0;

					return 0;
				}
			}

		}

		p = fgets(cpu.IR, 999, cpu.IP);
		cpuOffset++;
		pcbOffset++;

		if (p == NULL || feof(cpu.IP)) return 99; // end of program

		// printf(">>>%s\n",cpu.IR); // debug code

		result = prompt(cpu.IR);
		if (result != 0) return result;

		cpu.quanta--;
	}	

	// pcb->PC_offset = pcbOffset;
	return 0; // no errors
}
