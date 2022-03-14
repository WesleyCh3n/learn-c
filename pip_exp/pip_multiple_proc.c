#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void redirect(int old_fd, int new_fd) {
  if (new_fd == old_fd)
    return;
  dup2(old_fd, new_fd);
  close(old_fd);
}

void run(char *const args[], int in, int out) {
  redirect(in, STDIN_FILENO);
  redirect(out, STDOUT_FILENO);

  execvp(args[0], args);
}

int count_pipe_cmd(char cmd[]) {
  int length = 1;
  for (int i = 0; cmd[i] != '\0'; i++) {
    if (cmd[i] == '|')
      ++length;
  }
  return length;
}

void parse_pip(char cmd[], char **cmds[]) {
  char *p = strtok(cmd, "|");
  int i = 0;
  while (p != NULL) {
    printf("%s\n", p);
    strcpy(*cmds[i], p);
    p = strtok(NULL, "|");
    ++i;
  }
}

/* int main(int argc, char *argv[]) { */
int main(void) {
  char test[] = "echo Hello|grep e";
  char **test_cmds[count_pipe_cmd(test)];
  printf("length: %d\n", count_pipe_cmd(test));
  parse_pip(test, test_cmds);

  const char *cmd1[] = {"echo", "Hello 1\nHello 2\nNo 3", NULL};
  const char *cmd2[] = {"grep", "Hello", NULL};
  const char *cmd3[] = {"grep", "1", NULL};
  const char **cmds[] = {cmd1, cmd2, cmd3};
  int n = sizeof(cmds) / sizeof(*cmds);
  printf("# of cmd: %d\n", n);

  int fd[2];
  int in = STDIN_FILENO;
  pid_t pids[n];
  for (int i = 0; i < n - 1; ++i) {
    pipe(fd);
    if ((pids[i] = fork()) == 0) {
      run((char *const *)cmds[i], in, fd[1]);
    }
    close(fd[1]);
    in = fd[0];
  }

  run((char *const *)cmds[n - 1], in, STDOUT_FILENO);
  return 0;
}
