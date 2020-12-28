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

struct Node* head = NULL;

void doThis (char* passedin) {
    char* pass = strdup(passedin);
    char* command = strsep(&pass, "&");
    while (command != NULL) {
        if (strlen(command) > 0) {
        char* str = strdup(command);
        char* p = strsep(&str, ">\n\t ");  
        int i = 0;

        // initializing the array of arguments
        char** args = (char**)malloc(30 * sizeof(char*));
        for (int a = 0; a < 30; a++) {
          args[a] = (char*)malloc(20*sizeof(char));
        }
        
        char *redir = malloc(7 * sizeof(char));
        if (strstr(str, ">") != NULL) {
          redir = "true";
        }
        while (p!=NULL) { 
          if (strlen(p)>0 && (strncmp(p,">",1) != 0)){
            args[i] = p;  
            printf("arg #%d: %s\n",i, p);    
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
        else if (strncmp(args[0], "cd", 2) == 0) {
          int ret;
          ret = chdir(args[1]);
          if (ret == -1) {printEr();}
        }
         else if (strncmp(args[0], "path", 4) == 0) { // make linked list of strings that execv tries to execute 
            if (strncmp(args[1], "add", 3) == 0) {     // don't have to run execv on the path.
              struct Node* toAdd = (struct Node*)malloc(sizeof(struct Node)); 
            toAdd -> path = args[2];
            toAdd -> next = head;
            head = toAdd;
            printList(head);
          } 
          else if (strncmp(args[1], "remove", 6) == 0) {
            struct Node* curNode = (struct Node*)malloc(sizeof(struct Node));
            curNode = head; 
            struct Node* prevNode = (struct Node*)malloc(sizeof(struct Node));
            while (curNode != NULL) {
              if (strncmp(curNode -> path, args[2], strlen(args[2])) == 0) {
                if (curNode != head) {
                  prevNode->next = curNode -> next;
                  curNode = curNode->next;
                }
                else {
                  head = head->next;
                  curNode = head;
                }
              }
              else {
                prevNode = curNode;
                curNode = curNode->next;
              }
            }
            printList(head);
          }
          else if (strncmp(args[1], "clear", 5) == 0) {
            struct Node* curNode = (struct Node*)malloc(sizeof(struct Node));
            curNode = head;
            struct Node* next; 
            while (curNode!= NULL) {
              next = curNode -> next;
              free(curNode);
              curNode = next;
            }
            head = NULL;
            printList(head);
          }
        }
        else { // whileloop
         int c = 1;
         char *passin[i-1];
         int pid;
         if ((pid = fork()) == 0) {
          char * str = (char *) malloc(100);
          strcpy(str, "/bin/");
          strcat(str, args[0]);

          char * str2 = (char *) malloc(100);
          strcpy(str2, "/usr/bin/");
          strcat(str2, args[0]);

          if (access(str,F_OK | X_OK)==0) {
           passin[0] = str;  
         }
         else if (access(str2,F_OK | X_OK)==0) {
           passin[0] = str2;  
         }
         else {printEr();}
         printf("command is: %s\n",passin[0]);

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
          dup2(newfd, 1); 

          int exec_rc = execv( passin[0], passin );
          if (exec_rc != 0) { printEr();}
          fclose(new_f);
           } 
          else {
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
         exit(0);
         } else { wait(NULL);}
      }
     command = strsep(&pass, "&");
     }
    }
}

int main(int arg, char** argv) {
  char* line = NULL;
  size_t linecap = 0;
   ssize_t linelen;
  // pid_t child_pid;
  // int status = 0;

  // Initializing the linked list of paths.
  head = (struct Node*)malloc(sizeof(struct Node)); 
  head -> path = "/bin";
  head -> next = NULL;

 if (arg > 1) {
    printf("here4"); 
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
        doThis(command);
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
      if(strlen(command) > 0 && (strncmp(command, ";", 1) != 0)) {
        command[strlen(command)] = 0;
        printf("command is %s\n", command);
        doThis(command);
      }
      command = strsep(&commands, ";");
    }
      printf("smash> ");
  }
}
free(line);
return 0;
}
