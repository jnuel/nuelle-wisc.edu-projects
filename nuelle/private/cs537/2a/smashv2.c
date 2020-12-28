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

	// Initializing the linked list of paths.
	struct Node* head = (struct Node*)malloc(sizeof(struct Node)); 
	head -> path = "/bin";
	head -> next = NULL;

	// Starting the program!
    printf("smash> ");
	while((linelen = getline(&line, &linecap, stdin)) > 0) {
		char* str = strdup(line);
		char* p = strsep(&str, ">\n\t ");  
		
		char** args = (char**)malloc(30 * sizeof(char*));
		for (int a = 0; a < 30; a++) {
			args[a] = (char*)malloc(20*sizeof(char));
		}
		int i = 0;

		while (p!=NULL) { 
      if (strlen(p)>0 && (strncmp(p,">",1) != 0)){
			args[i] = p;  
      printf("%s\n", p);    
			i++;    
      }
      p = strsep(&str, "\n\t ");  
		}
    int numargs = i;

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
  			 }
  	 	  }
    else { // whileloop
      if (numargs == 2) { // how to tell number of args?? // wipe array and while haven't reachd the null keep loading in the arguments 
      printf("arg0 = %s\n", args[0]);
      printf("arg1 = %s\n", args[1]);
      char* arg0 = strdup(args[0]);
      args[0] = strsep(&arg0, "\n");

      char *passin[3];
      char * str = (char *) malloc(100);
      strcpy(str, "/bin/");
      strcat(str, args[0]);
      printf("%s\n", str);

      if (access(str, X_OK)==0) {
       passin[0] = str;   
     }
     else {printEr();}
     passin[1] = args[1]; // passin length = arglength; if arg[i] != null passin[i] = arg[i]
     passin[2] = NULL;   // if arg[i] null call execv 
     if (fork() == 0) {
       int exec_rc = execv( passin[0], passin );
       if (exec_rc != 0) { printEr(); printf("here");}
     }
    }
  else if (numargs == 1) {
      printf("arg0 = %s\n", args[0]);
      char* arg0 = strdup(args[0]);
      args[0] = strsep(&arg0, "\n");

      char *passin[2];
      char * str = (char *) malloc(100);
      strcpy(str, "/bin/");
      strcat(str, args[0]);
      printf("%s\n", str);

      if (access(str, X_OK)==0) {
       passin[0] = str;   
     }
     else {printEr();}
     passin[1] = NULL;
     if (fork() == 0) {
       int exec_rc = execv( passin[0], passin );
       if (exec_rc != 0) { printEr(); printf("here");}
     }
    }
  }
  	 	  printf("smash> ");
  	 	}
  	 	free(line);
  	 	return 0;
  	 }