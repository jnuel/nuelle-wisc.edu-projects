
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int arg, char** argue) {
printf(“starting\n”);
int rc = fork();
if (rc ==0) {
	printf(“Exiting from child %d and RC is %d\n”, getpid(), rc);
	
	// trying the exec file out. 
	char my_args[3];//creating an array of arguments you want to pass to exec
	my_args[0] = "/bin/ls";
	my_args[1] = "-la";
	my_args[2] = NULL;
	int exec_rc = execv("/bin/ls", my_args);

} else {
		//we have this RC which we can use to wait!
		int wait_rc = wait(NULL); // waits for the child process to finish and then prints output.
		printf(“Exiting from parent %d and RC is $d\n”, getpid(), rc);
}


return rc;
}
// successfully created a new process but then it is doing the same thing
//the other process is doing.. how to differentiate between these two processes?


// using strsep. 
int main() {
	char* str = strdup("This is a          very long line"); // strdup duplicates the string. we have to free it. don't need to duplicate it is it's already allocated on the heap. 
	// strsep does not work on constant strings. therefore it MUST be a dup!!

	printf("str is at %p\n", std);
	char* p = strsep(&str, " ");                    // takes in a pointer and a delimiter
	printf("str is at %p p is at %p\n", str, p);    // prints the location where the delimiter shows up
	while (p!=NULL) {  
	    if (strlen(p) > 0)                          // if we don't care about the delimiters,  
		printf("%s\n", p);                          // prints one token at a time! every space is printed as a separate token.
		p = strsep(%str, " ");					 	// to pass it along, keep passing in the same strsep as before. 
		printf("Str is at %p p is at %p\n", str, p);// with ls -l you can still do tokenization, turn it into an array, pass it in. 
	}

	free(str);
	return 0;
}


