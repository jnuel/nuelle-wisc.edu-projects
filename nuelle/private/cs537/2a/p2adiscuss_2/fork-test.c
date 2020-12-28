#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int arg, char** argv) {
  printf("Starting from %d\n", getpid());
  for (int i = 0; i < 2; i++) {
  int rc = fork();
  int status;
  if (rc == 0) {
    printf("Exiting from child and my PID is %d and RC is %d\n", getpid(), rc);
    sleep(3);
    char* my_args[3]; // HOW DO I KNOW THIS?
    my_args[0] = "/bin/wc";
    my_args[1] = "notes";
    my_args[2] = NULL;
    int exec_rc = execv("/bin/wc", my_args);
    printf("Done with exec!!!\n");
  } else {
    int wait_rc = waitpid(rc, &status, >0);
    printf("Exiting from parent and my PID is %d and RC is %d\n", getpid(), rc);
  }
}
  return 0;
}
