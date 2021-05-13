#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <time.h>

#include "pcb.h"
#include "kernel.h"
#include "ram.h"


// int findFrame(FILE *page);


// iterate through the ready queueu and find victim's frame number 
// then change it to null since we're taking it out of the memory 
void iterateRQ(int victimFrame){
	PCB *temp = getHead(); //get the head of the ready queue

	while(temp!=NULL) {

		//iterate through the pageTable of this PCB to check if it contains our victim frame number 
		// if so update it to -1
		for(int i = 0; i < 10; i++){
			if(temp->pageTable[i] == victimFrame){
				temp->pageTable[i] = -1; 
				break;
			}
		}
		temp = temp->next;
	}
}

// Assuming the frameNumber or victimNumber are negative if not used
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame){

	// bad programming practice ( nested loops )
	// update victim's PCB pageTable
	if(victimFrame >= 0){
		iterateRQ(victimFrame);
	}


	if(frameNumber >= 0 ){
		p->pageTable[pageNumber] = frameNumber;
	}else{
		p->pageTable[pageNumber] = victimFrame;
	}
	return 1;
}


// This function will update the RAM table with the page.
// It will choose the index of either frameNumber or vicimFrame
int updateFrame(int frameNumber, int victimFrame, FILE *page){
	if(frameNumber == -1){
		ram[victimFrame] = page;
	}else{
		ram[frameNumber] = page;
	}
}


int findVictim(PCB *p){
	

		//generate a random number in the interval [0,9]
		srand(time(NULL));
		int fn = rand() % 10; // random frame number (fn) between 0 and 9 

		int ret = -12; 

		//check if fn is in the PCB's page table
		for(int i = 0; i < 10; i++){
			if(p -> pageTable[i] == fn){
				ret = i; 
			}
		}

		while(1){
			//if ret is still -12, then return the fn, since no fn found in pageTable 
			if(ret == -12)
			{
				return fn;
			}
			else{ // the frame number is in the pageTable, so we have to find frame number that isn't 
				
				fn = (fn + 1) % 10; // increments until 9, then roll over to 0.

				//check if the new fn is in the pageTable 
				for(int i = 0; i < 10; i++){
					if(p -> pageTable[i] == fn){
						ret = i; 
					}
				}

			}

		}

	
}


//This function finds the first instance of non null cell and returns its index in ram array
//Attention: find the reason of having the file page as passed parameter.
int findFrame(FILE *page){

	//check that argument is valid
	if(page == NULL) return -1;

	// search for the first non null cell in ram array 
	for(int i = 0; i < 10; i++){
		if(ram[i] != NULL){
			return i;
		}
	}

	return -1;
}


//returns the total number of pages needed 
// 	1. Returns 1 -  if the program has 4 lines or less of code
//  2. Returns 2 -  if the program has ]4,8] lines of code
//  3. so on 

int countTotalPages(FILE *p){

	char ch;
	int totalNb;

	do
    {
        ch = fgetc(p);
        if(ch == '\n') totalNb++;   
    } while( ch != EOF ); 

    fclose(p);
    // fclose(f);

  	int div = totalNb / 4 ;

// if moduuls = 0, then it's the result of division
// otherwise divide totalNb by 4 and add 1
  	if(totalNb % 4 == 0){
  		if(div == 0 ){
  			return 1;
  		}else{
  			return div;
  		}
  	}else{
  		return (div+1); 
  	}
}

// this might not be working b/c of the pointer 
// Here, I am assuming that the pointer is now updated to the right line 
// ATTENTION: I can get an error related to pointer's duplication 	
FILE *findPage(int pageNumber, FILE *f){
	//dublicate the pointer
	//file descriptor 
	char filename[80];

	char d[50]; 
	int temp = dup(fileno(f));

	// fclose(fp2);
	// fclose(p);
	

	sprintf(d, "%d", temp) ;
		
	// find find the name of our script file
	char name[50];
	strcpy(name,"/proc/self/fd/" );
	strcat(name, d);
	ssize_t  r = readlink(name, filename, 50);
	

	//get rid of the path in filename
	char *fname = basename(filename);
	
	// printf("fname passed in fpage %s\n", fname );
	// create a command to copy a file in BackingStore
	char command[100];
	strcpy(command, "BackingStore/");
	strcat(command, fname);

	// printf("file name in bs %s\n", command);
	// printf("before oppening the file\n");


	FILE *fp2 = fopen( command, "r");
	
	// printf("after the fopen commnad  \n");
	int times = pageNumber * 4; 
	int i = 0; 
	char buf[250];
	while(i <= times ){
		fgets(buf,250,fp2);
		i++;
	}


	// fclose(f);
	return fp2;
}
int launcher(FILE *p){
	// char *temp;
	// strcpy(temp, p);
	
	// char name[100];
	// ssize_t  r = readlink("/proc/self/fd/p", filename, 50);
	// // char *filename = p;
	// strcpy(name, "cp ");
	// strcat(name, filename);
	// strcat(name, " /BackingStore");
 //    const char *n = name;
 //    printf("%s ---- filename\n", filename);
 //    printf("%s ---- name \n", name);
	// int temp = fileno(p);
	// printf("%d\n", temp);
	// FILE *pp = fdopen (dup (fileno (p)), "rt");
	// read file descriptor
	// printf("%p\n", p);
	char filename[60];


	FILE *fp2 = fdopen (dup (fileno (p)), "rt");

	

	//file descriptor 
	char d[50]; 
	int temp = dup(fileno(fp2));

	fclose(fp2);
	fclose(p);
	

	sprintf(d, "%d", temp) ;
		
	// find find the name of our script file
	char name[50];
	strcpy(name,"/proc/self/fd/" );
	strcat(name, d);
	ssize_t  r = readlink(name, filename, 51);
	

	//get rid of the path in filename
	char *fname = basename(filename);
	
	// create a command to copy a file in BackingStore
	char command[100];
	strcpy(command, "cp ");
	strcat(command, fname);
	strcat(command, " BackingStore");

	const char *n = command;

	// printf("%s\n", n);

    system(n);

    // printf("This is the before system(n) use \n");


    //open the file in BackingStore 
    // bp = Backingstore Pointer 
    FILE *bp = fopen(fname, "rt");

	return 1;
}