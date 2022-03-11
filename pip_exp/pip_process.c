#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Command:
//    echo "Hello 1\nHello 2\nNo 3" | grep Hello | grep 1
// Output:
//    Hello 1

int main() {
  int fd[2];
  pid_t pids[3];

  /* pipe2(fd, O_CLOEXEC); */
  pipe(fd);
  if ((pids[0] = fork()) == 0) {           // first child process
    dup2(fd[1], STDOUT_FILENO); // pipe stdout to fd write
    char *args[] = {"/bin/sh", "-c", "echo \"Hello 1 \nHello 2\nNo 3\"", NULL};
    execvp(args[0], args);
  }
  close(fd[1]); // close fd write
  close(fd[0]);

  pipe(fd);
  if ((pids[1] = fork()) == 0) {           // first child process
    dup2(fd[0], STDIN_FILENO);  // pipe stdin to fd read
    dup2(fd[1], STDOUT_FILENO); // pipe stdout to fd write
    char *args[] = {"/bin/sh", "-c", "grep Hello", NULL};
    execvp(args[0], args);
  }
  close(fd[1]); // close fd write
  close(fd[0]);

  if ((pids[2] = fork()) == 0) {           // first child process
    dup2(fd[0], STDIN_FILENO); // pipe stdin to fd read
    char *args[] = {"/bin/sh", "-c", "grep 1", NULL};
    execvp(args[0], args);
  }
  close(fd[1]); // close fd write
  close(fd[0]); // close fd write

  /* waitpid(pids[0], NULL, 0);
   * waitpid(pids[1], NULL, 0);
   * waitpid(pids[2], NULL, 0); */

  return 0;
}
