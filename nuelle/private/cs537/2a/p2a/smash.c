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

void printEr() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message)); 
}

struct Node* head = NULL;

void execthis(char** args, int i, char* redir) {
  if (i != 0) {
  int c = 1;
  int go = 1;
         char *passin[i-1];
         char* newcommand = NULL;
         int pid;
         if ((pid = fork()) == 0) {
          char * str = (char *) malloc(100);
           struct Node* cur = head;
           while (cur != NULL) {
            strcpy(str, cur->path);
            strcat(str, "/");
            strcat(str, args[0]);
            if (access(str,F_OK | X_OK)==0) {
                newcommand = str;  
                break;
              }
              cur = cur -> next;
           }
           passin[0] = args[0];

        if (newcommand == NULL) {
              printEr();go = 0;
           }

        if (go != 0) {
         if (strncmp(redir, "true", 4) == 0) {
           while (args[c] != NULL && c < i-1) {
            passin[c] = args[c];
            c++;
          }
          passin[c] = NULL;
          FILE* new_f = fopen(args[i-1], "w");
          int newfd = fileno(new_f);
          dup2(newfd, 1); 
          dup2(newfd,STDERR_FILENO);
          int exec_rc = execv( newcommand, passin );
          if (exec_rc != 0) { printEr();}
          fclose(new_f);
           } 
          else {
          while (args[c] != NULL && c < i) {
            passin[c] = args[c];
            c++;
          }
          passin[c] = NULL;
          int exec_rc = execv( newcommand, passin );
          if (exec_rc != 0) {printEr();}
         }
        }
         exit(0);
         } else { wait(NULL);}
         
  }
}

void builtin(char** args, int i, char* redir) {
  if(i!=0) {
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
          if(args[1] == NULL) {
            printEr();
          }
          else{
          ret = chdir(args[1]);
          if (ret == -1) {printEr();}
          }
        }
         else if (strncmp(args[0], "path", 4) == 0) { // make linked list of strings that execv tries to execute 
            if (strncmp(args[1], "add", 3) == 0) {     // don't have to run execv on the path.
              struct Node* toAdd = (struct Node*)malloc(sizeof(struct Node)); 
            toAdd -> path = args[2];
            toAdd -> next = head;
            head = toAdd;
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
          }
        }
        else { 
         execthis(args, i, redir);
        }
        }
}

void doThis (char* passedin) {
     char* pass = strdup(passedin);
     if (strstr(passedin, "&") != NULL) {
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
            i++;    
          }
          p = strsep(&str, ">\n\t ");  
        }
        execthis(args, i, redir);
      }
      command = strsep(&pass, "&");
    }
     } else {
             char* p = strsep(&pass, ">\n\t "); 
              int i = 0;

        //initializing the array of arguments
          char** args = (char**)malloc(30 * sizeof(char*));
          for (int a = 0; a < 30; a++) {
               args[a] = (char*)malloc(20*sizeof(char));
           }
        
          char *redir = malloc(7 * sizeof(char));
          if (strstr(passedin, ">") != NULL) {
              redir = "true";
          }
            while (p!=NULL) { 
               if (strlen(p)>0 && (strncmp(p,">",1) != 0)){
                 args[i] = p;   
                 i++;    
                }
           p = strsep(&pass, ">\n\t ");  
           }
           builtin(args, i, redir);
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
   FILE* f = fopen(argv[1], "r");
   if (f == NULL) {
    printEr();
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
   fflush(stdout);
  while((linelen = getline(&line, &linecap, stdin)) > 0) {
    char* commands = strdup(line);
     char* command = strsep(&commands, ";");

     while (command != NULL) {
       if(strlen(command) > 0 && (strncmp(command, ";", 1) != 0)) {
         if (strchr(command,'\n') != NULL)
         command[strlen(command)-1] = '\0';
         doThis(command);
       }
       command = strsep(&commands, ";");
    }
      printf("smash> ");
      fflush(stdout);
  }
}
free(line);
return 0;
}
