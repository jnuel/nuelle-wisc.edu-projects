////////////////////////////////////////////////////////////////////////////////
// Main File:        division
// This File:        division.c
// Other Files:
// Semester:         CS 354 Spring 2019
//
// Author:           Joy Nuelle
// Email:            nuelle@wisc.edu
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
//////////////////////////// 80 columns wide /////////////////////////////////////

/*
 * This program infinitely asks the user for two numbers to divide
 * and exits upon either the user entering 0 as the divisor, or 
 * the user hitting CTRL-C on their keyboard.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int success = 0; // counts the number of successful division operations

/*
 * This handler occurs when the user tries to divide by 0,
 * overrides the default divide by 0 exception by 
 * telling the user what happened, the number of successfully completed
 * operations, and then terminating instead of crashing the program.
 */
void div_handler() {
    printf("%s\n", "Error: a division by 0 operation was attempted.");
    printf("%s%d\n", "Total number of operations completed successfully: ",
           success);
    printf("%s\n", "The program will be terminated.");
    exit(0);
}

/*
 * This handler occurs when the user tries to CTRL-C out of the program and
 * cleanly ends the program, telling them the number of successful ops
 */
void sigint_handler() {
    printf("\n%s%d\n", "Total number of operations successfully completed: ",
           success);
    printf("%s\n", "The program will be terminated.");
    exit(0);
}

/*
 * Assigns the handlers appropriately, enters into an infinite 
 * while loop asking for two numbers to perform divide operations on.
 * It only ends if the user tries to divide by 0 or types CTRL-C.
 */
int main() {
    // setting up sigfpe
    struct sigaction sigfpe1;
    memset(&sigfpe1, 0, sizeof(sigfpe1));
    sigfpe1.sa_handler = div_handler;

    // registering sigusr1 to appropriate handler
    if (sigaction(SIGFPE, &sigfpe1, NULL) != 0) {
        fprintf(stderr, "Error registering handler function for SIGFPE.\n");
        exit(1);
    }

    // setting up sigint
    struct sigaction sigint1;
    memset(&sigint1, 0, sizeof(sigint1));
    sigint1.sa_handler = sigint_handler;
    
    // registering sigint to the appropriate handler
    if (sigaction(SIGINT, &sigint1, NULL) != 0) {
       fprintf(stderr, "Error registering handler function for SIGINT.\n");
       exit(1);
    }

    // entering the infinite while loop asking the user for two 
    //numbers to divide by.
    while(1) {
    char first[100];
    printf("Enter first integer: ");
    fgets(first, 100, stdin);
    int fnum = atoi(first);
    char second[100];
    printf("%s", "Enter second integer: ");
    fgets(second, 100, stdin);
    int snum = atoi(second);
    int divide = fnum/snum;
    int remainder = fnum % snum;
    printf("%d%s%d%s%d%s%d\n", fnum, " / ", snum, " is ", divide, 
        " with a remainder of ", remainder);
    success++;
    }
}
