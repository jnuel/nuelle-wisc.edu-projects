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
		char* p = strsep(&str, " "); //removing /n, tabs, strtok   
		
		char** args = (char**)malloc(30 * sizeof(char*));
		for (int a = 0; a < 30; a++) {
			args[a] = (char*)malloc(20*sizeof(char));
		}
		int i = 0;

		while (p!=NULL) { 
			p = strsep(&p, "\n");      
			args[i] = p;      
			i++;    
			p = strsep(&str, " ");					 	
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
		else if (strncmp(args[0], "ls", 2) == 0) {
			// add access
			char *args[2];
			if (access("/bin/ls", X_OK)==0)
  	 	  		args[0] = "/bin/ls";   
  	 	  	else if (access("/bin/ls", X_OK)==0) {
  	 	  		args[0] = "/usr/bin/ls";  
  	 	  	}
  	 	  	else { printEr(); }
  	 	  	args[1] = NULL; 
  	 	  	if (fork() == 0) {
  	 	  		int exec_rc = execv( args[0], args );
  	 	  		if (exec_rc != 0) { printEr(); printf("here");}
  	 	  	}
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
  	 	  printf("smash> ");
  	 	}
  	 	free(line);
  	 	return 0;
  	 }

