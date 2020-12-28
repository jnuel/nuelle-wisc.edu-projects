////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate
// This File:        intdate.c
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
 * This program prints out the current date and time every 3
 * seconds infinitely unless the user decides to send a SIGUSR1 signal
 * to the process, incrementing the count, or end the program by pressing
 * CTRL-C. When CTRL-C is called, it prints out how many times SIGUSR
 * was received.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int num_seconds = 3; // how often the alarm should go off
int ss1_count = 0; // how many times SIGUSR1 was successfully called

/*
 * This handler is called when the alarm goes off, and prints the 
 * current time and date of the computer.
 * int    sig  the amount of time the alarm should be set back to
 */
void handle(int sig) {
    time_t curtime;

    if (time(&curtime) == -1) {
       fprintf(stderr, "Error getting the current time.\n");
       exit(1);
    }

    char result[80];
    if (strcpy(result, ctime(&curtime)) == NULL) {
       fprintf(stderr, "Error getting the current time.\n");
       exit(1);
    }

    pid_t pid = getpid();
    printf("%s%ld%s%s", "PID: ",(long)pid, " | Current Time: ", result);
    alarm(num_seconds);
}

/*
 * This handler is accessed when SIGUSR1 is called!
 * It increments the number of times that we have successfully received SIGUSR1.
 */
void s1_handler() {
    printf("%s\n", "SIGUSR1 caught!");
    ss1_count++;
}

/*
 * This handler is called when CTRL-C is pressed on the keyboard.
 * It prints out how many times SIGUSR1 was received, and exits cleanly
 * instead of crashing.
 */
void sigint_handler() {
    printf("\n%s\n", "SIGINT received.");
    printf("%s%d%s\n", "SIGUSR1 was received ", ss1_count, 
        " times. Exiting now.");
    exit(0);
}

/*
 * This main sets up the handlers and assigns them appropriately, 
 * and also enters into an infinite while loop printing the 
 * time and date and PID until the user presses CTRL-C or calls SIGUSR1.
 */
int main() { 
    // setting up the alarm
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handle;

    // registering the alarm to the appropriate handler
    if (sigaction(SIGALRM, &act, NULL) != 0) {
       fprintf(stderr, "Error registering handler function for SIGALRM.\n");
       exit(1);
       }

    // setting up sigusr1
    struct sigaction signal1;
    memset(&signal1, 0, sizeof(signal1));
    signal1.sa_handler = s1_handler;

    // registering sigusr1 to appropriate handler
    if (sigaction(SIGUSR1, &signal1, NULL) != 0) {
        fprintf(stderr, "Error registering handler function for SIGSUR1.\n");
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

    printf("%s", "Pid and time will be printed every 3 seconds.\n");
    printf("%s", "Enter ^C to end the program.\n");
    alarm(num_seconds);

    while(1){
    }
}
