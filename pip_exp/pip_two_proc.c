#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void pipeline(char *args1, char *args2, char *args3) {
  int fd[2];
  pid_t pid[3];
  int in;

  pipe2(fd, O_CLOEXEC);
  printf("pipe: (r: %d, w: %d)\n", fd[0], fd[1]);
  if ((pid[0] = fork()) == 0) {
    dup2(STDIN_FILENO, STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    char *args[] = {"/bin/sh", "-c", args1, NULL};
    execvp(args[0], args);
  } else {
    printf("proc1: (r: %d, w: %d)\n", STDIN_FILENO, fd[1]);
    printf("Wait 1: %d\n", waitpid(pid[0], NULL, 0));
    close(fd[1]);
    in = fd[0];
  }

  pipe2(fd, O_CLOEXEC);
  printf("pipe: (r: %d, w: %d)\n", fd[0], fd[1]);
  if ((pid[1] = fork()) == 0) {
    dup2(in, STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    char *args[] = {"/bin/sh", "-c", args2, NULL};
    execvp(args[0], args);
  } else {
    printf("proc2: (r: %d, w: %d)\n", in, fd[1]);
    printf("Wait 2: %d\n", waitpid(pid[1], NULL, 0));
    close(fd[1]);
    in = fd[0];
  }

  pipe2(fd, O_CLOEXEC);
  printf("pipe: (r: %d, w: %d)\n", fd[0], fd[1]);
  if ((pid[2] = fork()) == 0) {
    dup2(in, STDIN_FILENO);
    dup2(STDOUT_FILENO, STDOUT_FILENO);
    char *args[] = {"/bin/sh", "-c", args3, NULL};
    execvp(args[0], args);
  } else {
    printf("proc3: (r: %d, w: %d)\n", in, STDOUT_FILENO);
    printf("Wait 3: %d\n", waitpid(pid[2], NULL, 0));
    close(fd[1]);
  }
  /* printf("pid1: %d, pid2: %d\n", pid[0], pid[1]); */
}

int main(void) {
  char args1[] = "echo \"Wesley\nChen\"";
  char args2[] = "grep e";
  char args3[] = "grep C";
  pipeline(args1, args2, args3);
  return 0;
}
