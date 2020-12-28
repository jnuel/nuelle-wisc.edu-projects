////////////////////////////////////////////////////////////////////////////////
// Main File:        Check Queens
// This File:        (name of this file)
// Other Files:      (name of all other files if any)
// Semester:         CS 354 SPRING 2019
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

char *COMMA = ",";

/* COMPLETED:
 * Retrieves from the first line of the input file,
 * the number of rows and columns for the board.
 * 
 * fp: file pointer for input file
 * rows: pointer to number of rows
 * cols: pointer to number of columns
 */
void get_dimensions(FILE *fp, int *rows, int *cols) {  
	char *line = NULL;  
	size_t len = 0;  
	if (getline(&line, &len, fp) == -1) {  
		printf("Error in reading the file\n");  
		exit(1);  
	}  

	char *token = NULL; 
	token = strtok(line, COMMA);
	*rows = atoi(token); 

	token = strtok(NULL, COMMA); 
	*cols = atoi(token);
}      


/* TODO:
 * Returns 1 if and only if there exists at least one pair
 * of queens that can attack each other.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * rows: number of rows
 * cols: number of columns
 */
int check_queens(int **board, int rows, int cols) { 
	int numqueens = 0; 

	    //Tests the columns to see if there is >1 queen in a given row. 
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
				if (*(*(board + i) + j) == 1) { //THERE IS A QUEEN THERE
					numqueens++;
				}
			}
			if (numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}

		numqueens = 0;

		//Tests the rows to see if there is >1 queen in a given column. 
		for (int j = 0; j < cols; j++) {
			for (int i = 0; i < rows; i++) {
				if (*(*(board + i) + j) == 1) {
					numqueens++;
				}
			}
			if (numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}

		numqueens = 0;
		//Tests the leading diagonals first by row to see if >1 queen present.
		for (int i = rows-1; i >= 0; i--) {
			int rowTest = i;
			int colTest = 0;

			if (*(*(board + rowTest) + colTest) == 1){
				numqueens++;
			}

			while (rowTest + 1 <= rows - 1 && colTest + 1 <= cols -1 ) {
				if (*(*(board + rowTest + 1) + (colTest + 1)) == 1) {
					numqueens++;
					rowTest++;
					colTest++;
				}
				else {
					rowTest++;
					colTest++;
				}
			}
			if(numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}

		numqueens = 0;
		//Tests the leading diagonals second by column to see if >1 queen is present.
		for (int i = cols-1; i >= 0; i--) {
			int rowTest = 0;
			int colTest = i;

			if (*(*(board + rowTest) + colTest) == 1){
				numqueens++;
			}
			while (colTest + 1 <= cols -1 && rowTest + 1 <= rows - 1) {
				if (*(*(board + rowTest + 1) + (colTest + 1)) == 1) {
					numqueens++;
					rowTest++;
					colTest++;
				}
				else {
					rowTest++;
					colTest++;
				}
			}
			if(numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}
		//Tests the non-leading diagonals first by row to see if >1 queen present.
		for (int i = rows-1; i >= 0; i--) {
			int rowTest = i;
			int colTest = 0;

			if (*(*(board + rowTest) + colTest) == 1){
				numqueens++;
			}

			while (rowTest - 1 >= 0 && colTest + 1 <= cols -1 ) {
				if (*(*(board + rowTest - 1) + (colTest + 1)) == 1) {
					numqueens++;
					rowTest--;
					colTest++;
				}
				else {
					rowTest--;
					colTest++;
				}
			}
			if(numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}

		numqueens = 0;
		//Tests the non-leading diagonals second by column to see if >1 queen is present.
		for (int i = 0; i <rows-1; i++) {
			int rowTest = i;
			int colTest = cols-1;

			if (*(*(board + rowTest) + colTest) == 1){
				numqueens++;
			}
			while (colTest - 1 >= 0 && rowTest + 1 <= rows - 1) {
				if (*(*(board + rowTest + 1) + (colTest - 1)) == 1) {
					numqueens++;
					rowTest++;
					colTest--;
				}
				else {
					rowTest++;
					colTest--;
				}
			}
			if(numqueens > 1) {
				return 1;
			}
			numqueens = 0;
		}

		return 0;
	}     


/* PARTIALLY COMPLETED:
 * This program prints true if the input file has any pair
 * of queens that can attack each other, and false otherwise
 * 
 * argc: CLA count
 * argv: CLA value 
 */
	int main(int argc, char *argv[]) {        

        //TODO: Check if number of command-line arguments is correct.
		if (argc != 2) {
			fprintf(stderr, "Usage: ./check_queens <input_filename>\n");
			exit(1);
		}

        //Open the file and check if it opened successfully.
		FILE *fp = fopen(*(argv + 1), "r");
		if (fp == NULL) {
			printf("Cannot open file for reading\n");
			exit(1);
		}


        //Declare local variables.
		int rows, cols;


        //TODO: Call get_dimensions to retrieve the board dimensions.
		get_dimensions(fp, &rows, &cols);

        //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
		int **board;
		board = malloc(sizeof(int*)*rows);
		if (board == NULL) {
			printf("Couldn't initialize board");
			exit(1);
		}
		for (int i = 0; i < rows; i++) {
        	board[i] = malloc(sizeof(int)*cols);  
        	if (board[i] == NULL) { 
        		printf("Couldn't initialize board");
        		exit(1);
        	}
        }

        //Read the file line by line.
        //Tokenize each line wrt comma to store the values in your 2D array.
        char *line = NULL;
        size_t len = 0;
        char *token = NULL;
        for (int i = 0; i < rows; i++) {

        	if (getline(&line, &len, fp) == -1) {
        		printf("Error while reading the file\n");
        		exit(1);        
        	}

        	token = strtok(line, COMMA);
        	for (int j = 0; j < cols; j++) {
                        //TODO: Complete the line of code below
                        //to initialize your 2D array.
                        *(*(board + i) + j) = atoi(token); 
                        token = strtok(NULL, COMMA);    
                    }
                }

        //TODO: Call the function check_queens and print the appropriate
        //output depending on the function's return value.
                int attackPossible = check_queens(board,rows,cols);
                if (attackPossible == 1 ) {
                	printf("true\n");
                }
                else {
                	printf("false\n");
                }

        //TODO: Free all dynamically allocated memory.
                for(int i = 0; i < rows; i++){
        	free(board[i]); 
        }
        free(board);


        //Close the file.
        if (fclose(fp) != 0) {
        	printf("Error while closing the file\n");
        	exit(1);        
        }

        return 0;
    }      
