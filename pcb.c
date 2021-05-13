#include<stdlib.h>
#include<stdio.h>
#include "pcb.h"
#include "memorymanager.h"
#include <unistd.h>
// Global data structure

PCB *head = NULL, *tail=NULL;

void printPCB() {
	PCB *temp = head;

	printf("Addr:");

	while(temp!=NULL) {
		printf(" %d",temp->start);
		temp = temp->next;
	}

	printf("\n");
}
// exec script.txt
PCB *makePCB(FILE *p, int startAddressRAM) {
	PCB *pcb;


	if (p == NULL) return NULL;

	pcb = (PCB *) malloc(sizeof(PCB*));
	if (pcb == NULL) return NULL;

	pcb->PC    = p;

	// printf("This is the file pointer before the counting of total number of pages %p\n", pcb->PC);
	// FILE *f = fdopen (dup (fileno (p)), "r");
	// pcb->pages_max = countTotalPages(f);
	pcb->pages_max = 2; 
    
	// printf("This is the f pointer while %p\n", f);
    
    // fclose(f);
    // printf("This is the file pointer after countin the total nb of pages %p\n", pcb->PC );
	pcb->start     = startAddressRAM;
	pcb->next      = NULL;
	pcb->PC_offset = 0;
	pcb->PC_page   = 0;

	// fclose(p);
	// intialize pages_max atribute
	// int i = countTotalPages(p);
	// printf("%d\n", pcb->pages_max);
	
	

}

void addToReady(PCB *p) {
	
	// printf("This is add to ready queue pointer %p\n", p->PC);

	p->next = NULL;
	if (head == NULL) {
		head = p;
		tail = p;
	} else {
		tail->next = p;
		tail = p;
	}
}

PCB *getHead() {
	return head;
}
PCB *getFromReady() {
	PCB *temp;

	if (head == NULL) return NULL;

	temp = head;
	head = head->next;
	temp->next = NULL;

	if (head == NULL) tail = NULL;

	return temp;
}

