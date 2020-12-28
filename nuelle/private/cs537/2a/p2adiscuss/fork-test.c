#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int arg, char** argv) {

  
  printf("Starting from %d\n", getpid());
  int rc = fork();
  if (rc == 0) {
    printf("Exiting from child and my PID is %d and RC is %d\n", getpid(), rc);
    char* my_args[3]; // HOW DO I KNOW THIS?

    printf("arg1 = %s\n", argv[1]);
    printf("arg2 = %s\n", argv[2]);
    char* arg1 = strdup(argv[1]);
    argv[1] = strsep(&arg1, " \n");

    my_args[0] = "wc";
    my_args[1] = "notes.txt";
    my_args[2] = NULL;

    char * str = (char *) malloc(100);
      strcpy(str, "/bin/");
      printf("arg1 = %s\n", argv[1]);
      strcat(str, argv[1]);
      printf("str = %s\n", str);

    int exec_rc = execv(str, my_args);
    printf("%d\n", exec_rc);
    printf("Done with exec!!!\n");
  } else {
    int wait_rc = wait(NULL);
    printf("%d\n", wait_rc);
    printf("Exiting from parent and my PID is %d and RC is %d\n", getpid(), rc);
  }

  return rc;
}
