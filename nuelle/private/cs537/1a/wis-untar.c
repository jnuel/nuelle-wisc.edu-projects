#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
  char* filename = argv[1];
  char name[100];
  char *contents;

  if (argc <= 1 || argc > 2) {
    printf("wis-untar: tar-file\n");
    exit(1);
  }

  filename = argv[1];

	// Open the file and read lines from the file
  FILE* f = fopen(filename, "r");
  if (f==NULL) {
   printf("wis-untar: cannot open file\n");
   exit(1);
 }
// printf("opening file: %s\n", filename);

while (fread(name, 100, 1, f) != 0) {
 //printf("file name: %s\n", name);

  off_t num = 0;
 fread(&num, sizeof(off_t), 1, f);
 //printf("file size: %ld\n", num);


 contents = (char*)calloc(num, sizeof(char));
 fread(contents, num, 1, f);
 //printf("contents: %s\n", contents);
 FILE * opened = fopen(name, "w");
 fwrite(contents, num, 1, opened);
 fclose(opened);
}
 free(contents);
 fclose(f);
 return 0;
}
