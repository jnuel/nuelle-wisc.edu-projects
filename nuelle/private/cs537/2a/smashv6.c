#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

struct Node { 
  char* path; 
  struct Node* next; 
}; 

void printList(struct Node* n) 
{ 
  while (n != NULL) { 
    printf(" %s ", n->path); 
    n = n->next; 
  } 
  printf("\n");
} 

 // Initializing the linked list of paths.
struct Node* head = NULL; 

void printEr() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message)); 
}

char* redirection(char* str) {
    printf("str is %s", str);
    if (strstr(str, ">") != NULL) {
        return "true";
      }
     return("false");
}

void separate(char* command) {
    char* redir = malloc(7 * sizeof(char));
    // if (strstr(and1, "&") != NULL) {
    //      //printf("%s contains &\n", and1);
    //      //separate the commands based off '&' and call execv
    //     char* and = strsep(&and1, "&");
    //      while (and != NULL) {
    //           if (strlen(and) > 0) {
    //             char* str = strdup(and);
    //             char* p = strsep(&str, ">\t ");  
    //              char** args = (char**)malloc(30 * sizeof(char*));

    //             for (int a = 0; a < 30; a++) {
    //                 args[a] = (char*)malloc(20*sizeof(char));
    //              }
    //           int i = 0;
    //           while (p!=NULL) { 
    //             if ((strlen(p) > 0)) {
    //                  args[i] = p;  
    //                  printf("arg #%d: %s\n",i, p);    
    //                  i++;    
    //              }
    //              p = strsep(&str, ">\t ");  
    //           }
    //         }
    //         redir = redirection(and);
    //             if (strncmp(redir, "true", 4)){
    //                 printf("this command %s returned true!\n", and);
    //             }
    //           //  CALL EXECV AND FORK!
    //           and = strsep(&and1, "&");
    //      }
    // }
    // else {
         char* str1 = strdup(command);
         char* str = strsep(&str1, ">\t ");
          while (str!= NULL) {
              if (strlen(str)>0) { 
            int i = 0;
            char** args = (char**)malloc(30 * sizeof(char*));

            for (int a = 0; a < 30; a++) {
                args[a] = (char*)malloc(20*sizeof(char));
                }
             if (strstr(str1, ">") != NULL) {
                    redir = "true";
                }
                
                args[i] = str;  
                printf("arg #%d: %s\n",i, str);    
                i++;    
             }
                str = strsep(&str1, ">\t ");  
            }
            if (strncmp(redir, "true", 4)){
                    printf("str returned true!\n");
                }
           // call built in with execv and fork in it
        // separate the commands based off other and run them
    //}
}

int main(int arg, char** argv) {
  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  head = (struct Node*)malloc(sizeof(struct Node)); 
  head -> path = "/bin";
  head -> next = NULL;

  // Starting the program!

  if (arg > 1) {
    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
       printEr();
       exit(1);
    }

  while((linelen = getline(&line, &linecap, f) > 0)) {
    char* commands = strdup(line);
    char* command = strsep(&commands, ";");

    while (command != NULL) {
      if(strlen(command) > 0) {
        separate(command);
      }
      command = strsep(&commands, ";");
    }
  }
  fclose(f);
} else {
   printf("smash> ");
  while((linelen = getline(&line, &linecap, stdin)) > 0) {
    char* commands = strdup(line);
    char* command = strsep(&commands, ";");

    while (command != NULL) {
      if(strlen(command) > 0) {
        command[strlen(command)] = 0;
        printf("command is %s\n", command);
        separate(command);
      }
      command = strsep(&commands, ";");
    }
      printf("smash> ");
  }
}
free(line);
return 0;
}


