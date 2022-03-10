#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* typedef struct process_t {
 *   pid_t pid;
 * }process_t;
 *
 * process_t subprocess(char *cmd) {
 *   int fd[2];
 *   pipe(fd);
 *   pid_t pid = fork();
 *   return (process_t){pid};
 * } */

int main() {
  int fd[2];
  pipe(fd);

  pid_t child_pid = fork();
  if (child_pid == 0) {         // first child process
    /* close(fd[0]);               // close fd read */
                                //
    dup2(fd[1], STDOUT_FILENO); // pipe stdout to fd write
    close(fd[1]);               // close fd write

    char *args[] = {"/bin/sh", "-c",
                    "echo \"Hello from \nwesley overflow\nHe again\"", NULL};
    execvp(args[0], args);
  }

  child_pid = fork();
  if (child_pid == 0) {
    dup2(fd[0], STDIN_FILENO); // pipe stdin to fd read
    /* close(fd[0]);              // close fd read */

    dup2(fd[1], STDOUT_FILENO); // pipe stdout to fd write
    close(fd[1]);               // close fd write

    char *args[] = {"/bin/sh", "-c", "grep He", NULL};
    execvp(args[0], args);
  }

  close(fd[1]);
  dup2(fd[0], STDIN_FILENO); // pipe stdin to fd read
  close(fd[0]);              // close fd read


  char *args[] = {"/bin/sh", "-c", "grep again", NULL};
  execvp(args[0], args);
  /* close(fd[0]); */

  return 0;
}
