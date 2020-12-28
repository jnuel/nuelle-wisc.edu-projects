////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig
// This File:        sendsig.c
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
 * This program allows the user to send the signals kill or sigusr
 * to a different process by typing in the process id and the signal
 * that they want to send
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/*
 * This sets up the format for the user to send signals(u or i) to any process.
 */
int main(int argc, char*argv[]) {
    if (argc != 3) {
        printf("Usage: <signal type> <pid>\n");
        exit(0);
    }

    int result = strcmp(argv[1], "-i");
    pid_t pid = atoi(argv[2]);
  
    if (result > 0) {
        if (kill(pid, SIGUSR1) == -1) {
            printf("Error trying to send sigusr1\n");
            exit(1);
        }
    } else if (result == 0) {
        if (kill(pid, SIGINT) == -1) {
            printf("Error trying to send sigint\n");
            exit(1);
        }
    } else {
      printf("Invalid args\n");
      exit(0);
    }

    return 0;
}
