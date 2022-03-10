#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  pipe(fd);
  pid_t child_pid = fork();
  if (child_pid == 0) {         // first child process
    close(fd[0]);               // close fd read

    dup2(fd[1], STDOUT_FILENO); // pipe stdout to fd write
    close(fd[1]);               // close fd write

    char *args[] = {"/bin/sh", "-c", "echo \"Hello from \nwesley overflow\"",
                    NULL};
    execvp(args[0], args);
  } else {
    child_pid = fork();
  }
  if (child_pid == 0) {
    close(fd[1]); // close fd write

    dup2(fd[0], STDIN_FILENO); // pipe stdin to fd read
    close(fd[0]); // close fd read

    char *args[] = {"/bin/sh", "-c", "grep He", NULL};
    execvp(args[0], args);
  }

  /* int status; */
  /* waitpid(child_pid, &status, WUNTRACED); */
  printf("Test\n");
  close(fd[1]);
  /* char buf[15]; */
  /* read(fd[0], buf, sizeof(buf)); // this will wait for buf full */
  /* printf("Read from pipe bridge process: %s\n", buf); */
  close(fd[0]);

  return 0;
}
