////////////////////////////////////////////////////////////////////////////////
// Main File:        Mem.c
// This File:        Mem.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Spring 2019
//
// Author:           Joy Nuelle
// Email:            (nuelle@wisc.edu
// CS Login:         nuelle
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct block_header {
	int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27 // 11   
    *      If the previous block is free, size_status should be 25      // 9
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26 // 10
    *      If the previous block is free, size_status should be 24      // 8
    *    Footer:
    *      size_status should be 24
    */
} block_header;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */  

block_header *start_block = NULL;

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use BEST-FIT PLACEMENT POLICY to find the block closest to the required block size
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic.
 */
void* Alloc_Mem(int size) {
	block_header *current = start_block; //the current block
	block_header *bestFit = NULL; // points to the block that has the best fit so far 
	void* ptr = NULL; 	// the pointer to be returned
	int size_status; 	// so that I don't have to type current -> size_status each time
	int actual_size; 	// will hold the block's raw status(minus p an a bits)
	int padsize;		// to calculate the padding (see below)
	int alloc_size;  	// the size desired plus the padding (see below)

	if (size <=0) {
		return NULL;
	}

    size = size + (sizeof(block_header*)); // includee the amount of bytes for block header
    padsize = size % 8;
    padsize = (8 - padsize) % 8;

    // determine block size rounding up multiple of eight, adding padding.
    alloc_size = size + padsize;


    while (current->size_status != 1 ) { //WHILE YOURE NOT AT THE LAST BLOCK
	 if (current -> size_status > alloc_size) {
		if(current -> size_status % 2 == 0){ // even, means that it is currently free!
			if (bestFit == NULL ) { // set the bestfit if it doesn't have anything before
					bestFit = current;
					bestFit -> size_status = current -> size_status;
    			}
			else if ( bestFit -> size_status > current -> size_status) { // else compare the bestfit to the current, save if current's block is better
					bestFit = current;
					bestFit -> size_status = current -> size_status;
				}
    	} 
			}
			size_status = current -> size_status; // go to the next block
    		actual_size = size_status - (size_status & 3);
			current = (block_header*)((char*)current +  actual_size);
	}

	//Decide whether or not to split
	if (bestFit != NULL) {
		block_header* new;
		block_header* footer;
		int totalSpace = bestFit -> size_status - (bestFit -> size_status & 3);
		int previousbusy = (bestFit -> size_status & 2);
		int bestFitssize = bestFit -> size_status - (bestFit -> size_status % 8);

		//if you need to split
		if (bestFit -> size_status > alloc_size && bestFit -> size_status - alloc_size >= 8) { 
			size_status = bestFit -> size_status; 
			bestFit -> size_status = alloc_size + (size_status & 2) + 1; // added a 1 to say allocated	
			ptr = (block_header*)((char*)bestFit + sizeof(block_header));
			totalSpace = totalSpace - alloc_size; 
			new = (block_header*)((char*)bestFit + alloc_size); 
			new -> size_status = totalSpace + 2; // because already allocated
			footer = (block_header*)((char*)new + totalSpace - sizeof(block_header));
    		if(footer -> size_status != 1) {
			footer -> size_status = totalSpace;
    		}

		}
		//you don't need to split
		else {
			bestFit -> size_status = bestFitssize + previousbusy + 1;
		ptr = (block_header*)((char*)bestFit + sizeof(block_header));
		//fixing the next block's header to say previous block allocated
		block_header *next = (block_header*)((char*)bestFit + alloc_size); 

		if (next -> size_status != 1) { // fix the next block's status
				next -> size_status += 2;
				}
		}
	}

	return ptr;
}

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int Free_Mem(void *ptr) { 
	block_header *current = start_block;
	while ( current -> size_status != 1) { // loop to find the end
			int size_status = current -> size_status;
    		int actual_size = size_status - (size_status & 3);	
			current = (block_header*)((char*)current +  actual_size);
	}

	block_header* end = current; // set the end of the entire block
	current = ptr - sizeof(block_header*); // reassign current to the ptr's address - the size of block header

	if(ptr == NULL){
		return -1;
	}
	else if ((unsigned int)ptr % 8 != 0) {
		return -1;
	}
	else if (current >= end) {
		return -1;
	}
	else if (current -> size_status % 2 == 0) { // even, it is already free!
		return -1;
	}
	else if (current -> size_status % 2 != 0) { // odd, needs to be free.
		int size_status = (current -> size_status) & 3; // going to be either 1 or 3. if 1, previous not allocated. 
		int actual_size = (current -> size_status) - size_status;

	//going to the next block to see if its free or not
		block_header *next = (block_header*)((char*)current + actual_size); 
				if ((next -> size_status & 1) ==0) { // if it is even, it is free. 
					int nextactualsize = next -> size_status - (next -> size_status & 3);
					actual_size += nextactualsize; // stores what current's size status should end up as...
				}

	// checking the previous block if it is free or not
			if ((current -> size_status & 2) == 0) { //previous block is free, can coalesce
				block_header *previousfooter = (block_header*)((char*)current - sizeof(block_header));
				block_header *previousheader = (block_header*)((char*)current - (previousfooter -> size_status));
				actual_size += previousfooter -> size_status;
				previousheader -> size_status = actual_size + 2; // because now allocated
				block_header *footer = (block_header*)((char*)previousheader + (previousheader -> size_status) - 2 - sizeof(block_header));
				if(footer -> size_status != 1) {
					footer -> size_status = actual_size;
					}	

			block_header *next = (block_header*)((char*)previousheader + actual_size); 
				//testing to see if the next block's size status needs to be changed or not
			 if (next -> size_status != 1 && (next -> size_status & 2) == 1) {
				next -> size_status += 2;
				}
			else if (next -> size_status != 1 && (next -> size_status & 2) == 2) {
				next -> size_status -= 2;
				}
			}
	// previous block is allocated, can't coalesce... 
			else {   
				actual_size += 2;
				current -> size_status = actual_size;
				actual_size = actual_size - (actual_size & 3);
				block_header *footer = (block_header*)((char*)current + actual_size - sizeof(block_header));
				if (footer -> size_status != 1) {
				footer -> size_status = actual_size;
				}	
			//testing to see if the next block's size status needs to be changed or not
			block_header *next = (block_header*)((char*)current + actual_size); 
			 if (next -> size_status != 1 && (next -> size_status & 2) == 1) {
				next -> size_status += 2;
				}
			else if (next -> size_status != 1 && (next -> size_status & 2) == 2) {
				next -> size_status -= 2;
				}
			}
		return 0;
	}
	return -1;
}

/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int Init_Mem(int sizeOfRegion) {         
	int pagesize;
	int padsize;
	int fd;
	int alloc_size;
	void* space_ptr;
	block_header* end_mark;
	static int allocated_once = 0;

	if (0 != allocated_once) {
		fprintf(stderr, 
			"Error:mem.c: Init_Mem has allocated space during a previous call\n");
		return -1;
	}
	if (sizeOfRegion <= 0) {
		fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
		return -1;
	}

    // Get the pagesize
	pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;

	alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
	fd = open("/dev/zero", O_RDWR);
	if (-1 == fd) {
		fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
		return -1;
	}
	space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
		fd, 0);
	if (MAP_FAILED == space_ptr) {
		fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
		allocated_once = 0;
		return -1;
	}

	allocated_once = 1;

    // for double word alignment and end mark
	alloc_size -= 8;

    // To begin with there is only one big free block
    // initialize heap so that start block meets 
    // double word alignement requirement
	start_block = (block_header*) space_ptr + 1;
	end_mark = (block_header*)((void*)start_block + alloc_size);

    // Setting up the header
	start_block->size_status = alloc_size;

    // Marking the previous block as used
	start_block->size_status += 2;

    // Setting up the end mark and marking it as used
	end_mark->size_status = 1;

    // Setting up the footer
	block_header *footer = (block_header*) ((char*)start_block + alloc_size - 4);
	footer->size_status = alloc_size;

	return 0;
}          

/* F
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void Dump_Mem() {         
	int counter;
	char status[5];
	char p_status[5];
	char *t_begin = NULL;
	char *t_end = NULL;
	int t_size;

	block_header *current = start_block;
	counter = 1;

	int used_size = 0;
	int free_size = 0;
	int is_used = -1;

	fprintf(stdout, "************************************Block list***\
		********************************\n");
	fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
	fprintf(stdout, "-------------------------------------------------\
		--------------------------------\n");

	while (current->size_status != 1) {
		t_begin = (char*)current;
		t_size = current->size_status;

		if (t_size & 1) {
            // LSB = 1 => used block
			strcpy(status, "used");
			is_used = 1;
			t_size = t_size - 1;
		} else {
			strcpy(status, "Free");
			is_used = 0;
		}

		if (t_size & 2) {
			strcpy(p_status, "used");
			t_size = t_size - 2;
		} else {
			strcpy(p_status, "Free");
		}

		if (is_used) 
			used_size += t_size;
		else 
			free_size += t_size;

		t_end = t_begin + t_size - 1;

		fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
			p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

		current = (block_header*)((char*)current + t_size);
		counter = counter + 1;
	}

	fprintf(stdout, "---------------------------------------------------\
		------------------------------\n");
	fprintf(stdout, "***************************************************\
		******************************\n");
	fprintf(stdout, "Total used size = %d\n", used_size);
	fprintf(stdout, "Total free size = %d\n", free_size);
	fprintf(stdout, "Total size = %d\n", used_size + free_size);
	fprintf(stdout, "***************************************************\
		******************************\n");
	fflush(stdout);

	return;
}   