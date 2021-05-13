typedef struct PCB_REC {
	FILE *PC;  // offset from the beginning of a frame, where the offset is the line count from 0. || Current position in the file. 	
	int start; // startAddressRAM
	struct PCB_REC *next;
	int pageTable[10];
	int PC_page;
	int PC_offset;
	int pages_max;
} PCB;

void addToReady(PCB *p);
PCB *getFromReady();
PCB *makePCB(FILE *p, int startAddressRAM);
PCB *getHead();
void printPCB();
