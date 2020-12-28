#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
	char *line = NULL;
	char* filename = argv[1];

    // Checking to make sure appropriate num of args is present
	if (argc <= 2) {
		printf("wis-tar: tar-file file […]\n");
		exit(1);
	}

	// Open the file that is going to be tarred
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printEr();
        exit(1);
    }
	
    // Opening the other files we need to grab info from.
	int num = 2;
	while(argv[num] != NULL) {
		char* fname = argv[num];
//        printf("%s\n", fname);
    	// Open the file and read lines from the file
		FILE* toread = fopen(fname, "r");
		if (toread==NULL) {
			printf("wis-tar: cannot open file\n");
			exit(1);
		}

        char *buffer = (char*)calloc(100, sizeof(char));	
        if (buffer == NULL) {
            return 1;
        }

		fread(buffer, 1, 100, toread);
		line = strtok(strdup(buffer), "\n");
                
			if (strlen(fname) < 100) {
//			    printf("in the fputs for fname%s\n", fname);
                fputs(fname, f);
				for (int i = 0; i < 100-strlen(fname); i++){
					fwrite("\0", 1, 1, f);
				}
			//	printf("done writing file name%s\n", fname);
			//	printf("finished writing the long loop!");
			}
			else if (strlen(fname) >= 100) {
				fwrite(fname, 100, 1, f);
//                printf("in the fwrite for fname >= 100%s\n", fname);
//				line = strtok(NULL, "\n");
			}	
		struct stat info;
    	int err = stat(fname, &info);
    	if (err == -1) {
    		printf("wis-tar: something wrong with file size\n");
    		exit(1);
    	}
    	// Do error checking here!

    	//printf("size of a.txt = %lld\n", info.st_size);
		long filesize = info.st_size;
//        printf("in the file size writing%ld\n", filesize);
		fwrite(&filesize, sizeof(long), 1, f);
	//	printf("done writing file size");
       
			while (line) {
  //              printf("in the line write%s\n", buffer);
				fputs(buffer,f);
			//	printf(" %sLINE CONTENTS\n", line);
				line = strtok(NULL, "\n");
			}
		fclose(toread);
		num++;
        free(buffer);
	}

	// long filesize = 6;
	// // Write out a number as ASCII
	// // fprintf(f, "%ld", filesize);


	// // Write out a number as binary
	// fwrite(&filesize, sizeof(long), 1, f); // Why is there this 1 here? what is the size of one member of this pointer? 
	// 									// size of the data, how many items of data we want to write.

	fclose(f);

	return 0;
}
