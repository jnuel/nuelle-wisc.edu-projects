#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
 #define MAX 10 


int main(int argc, char** argv) {
	// argc number of arguments
	// argv string array with one for each argument that is in arg
	size_t linecap = 0;
	ssize_t linelen;
	char *line = NULL;
     char *buffer = (char*)calloc(1024, sizeof(char));

	// First element in argv is the program name

        if (argc < 2) {
          printf("wis-grep: searchterm [file ...]\n");
          exit(1);
  }


  char* search_term = argv[1];
  int num = 2;
  if (argc == 2) {
        fread(buffer, 1, 1024, stdin);
                line = strtok(strdup(buffer), "\n");
                while (line) {
                        if (strstr(line, search_term) != NULL)
                        printf("%s\n", line);
                        line = strtok(NULL, "\n");
                }
        }


while(argv[num] != NULL) {
    char* filename = argv[num];

	// Open the file and read lines from the file
    FILE* f = fopen(filename, "r");
    if (f==NULL) {
           printf("wis-grep: cannot open file\n");
           exit(1);
   }

   while((linelen = getline(&line, &linecap, f)) > 0) {
           if(strstr(line, search_term) != NULL) {
                  fwrite(line, linelen, 1, stdout);
          }
  }

  fclose(f);
  num++;
}

free(line);
free(buffer);
return 0;
}
