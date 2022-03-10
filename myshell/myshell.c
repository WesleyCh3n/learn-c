#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

u_int gMaxLineLength = 256;

int c_execvp(char *cmd) {
  pid_t pid = fork();
  if (pid == 0) {
    char *args[] = {"/bin/sh", "-c", cmd, NULL};
    execvp(args[0], args);
    // child gets here only if there was an error
    perror("[CHILD] Error");
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("fork failed!");
  }

  int status;
  waitpid(pid, &status, WUNTRACED);
  return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
}

int main() {
  char command[gMaxLineLength];
  while (1) {
    printf("> ");
    fgets(command, sizeof(command), stdin);

    if (feof(stdin)) {
      break;
    }

    command[strlen(command) - 1] = '\0';

    int cmdReturnCode = c_execvp(command);
    printf("return code = %d\n", cmdReturnCode);
  }

  printf("\n");
  return 0;
}
