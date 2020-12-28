////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dcols
// This File:        cache2Dcols.c
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
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arr2D[3000][500];

//iterates through the array in col-wise order and sets each value to the sum of the col+row.
int main() {
	for (int col = 0; col < 500; col++) {
		for (int row = 0; row < 3000; row++) {
			arr2D[row][col] = row + col;
		}
	}
	return 0;
}
