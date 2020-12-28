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

void printEr() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int main(int arg, char** argv) {
  char* line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  pid_t child_pid;
  int status = 0;

  // Initializing the linked list of paths.
  struct Node* head = (struct Node*)malloc(sizeof(struct Node)); 
  head -> path = "/bin";
  head -> next = NULL;

  // Starting the program!
  printf("smash> ");
  while((linelen = getline(&line, &linecap, stdin)) > 0) {
    char* commands = strdup(line);
    char* command = strsep(&commands, "&");

    while (command != NULL) {
      if ((child_pid = fork())== 0) {
        char* str = strdup(command);
        char* p = strsep(&str, ">\n\t ");  

    // initializing the array of arguments
        char** args = (char**)malloc(30 * sizeof(char*));
        for (int a = 0; a < 30; a++) {
          args[a] = (char*)malloc(20*sizeof(char));
        }
        int i = 0;
        char *redir = malloc(7 * sizeof(char));
        if (strstr(str, ">") != NULL) {
          redir = "true";
        }
        while (p!=NULL) { 
          if (strlen(p)>0 && (strncmp(p,">",1) != 0)){
            args[i] = p;  
            printf("%s\n", p);    
            i++;    
          }
          p = strsep(&str, ">\n\t ");  
        }

        if(strncmp(args[0], "exit", 4) == 0) {
          for (int b = 0; b < 30; b++) {
            if (args[b]!=NULL)
              free(args[b]);
          }
          free(args);
          exit(0);
        }
    else { // whileloop
     int c = 1;
     char *passin[i-1];
     int pid;
     if ((pid = fork()) == 0) {
      char * str = (char *) malloc(100);
      strcpy(str, "/bin/");
      strcat(str, args[0]);
      printf("%s\n", str);

      char * str2 = (char *) malloc(100);
      strcpy(str2, "/usr/bin/");
      strcat(str2, args[0]);
      printf("%s\n", str2);

      if (access(str,F_OK | X_OK)==0) {
       passin[0] = str;  
     }
     else if (access(str2,F_OK | X_OK)==0) {
       passin[0] = str2;  
     }
     else {printEr();}
     printf("%s\n",passin[0]);

     if (strncmp(redir, "true", 4) == 0) {
       while (args[c] != NULL && c < i-1) {
        printf("args w redir %d = %s\n", c, args[c]);
        passin[c] = args[c];
        c++;
      }
      passin[c] = NULL;
      FILE* new_f = fopen(args[i-1], "w");
      printf("opening %s\n", args[i-1]);
      int newfd = fileno(new_f);

      //  FD number 1 is stdout
      // I want to replace it with newfd
      dup2(newfd, 1); 

      int exec_rc = execv( passin[0], passin );
      if (exec_rc != 0) { printEr();}
      fclose(new_f);

    } else {
      printf("hereee");
      while (args[c] != NULL && c < i) {
        printf("args no redir%d = %s\n", c, args[c]);
        passin[c] = args[c];
        c++;
      }
      passin[c] = NULL;
      int exec_rc = execv( passin[0], passin );
      if (exec_rc != 0) {printEr();}
    }
  }
}
printf("smash> ");
}
else {
  waitpid(child_pid, &status, 0); 
}
command = strsep(&commands, "&");
}
}
free(line);
return 0;
}