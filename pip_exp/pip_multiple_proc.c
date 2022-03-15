#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void redirect(int old_fd, int new_fd) {
  if (new_fd == old_fd)
    return;
  dup2(old_fd, new_fd);
  close(old_fd);
}

void run(char *const cmd, int in, int out) {
  redirect(in, STDIN_FILENO);
  redirect(out, STDOUT_FILENO);

  char *args[] = {"/bin/sh", "-c", cmd, NULL};
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

char **split_cmds_new(char cmd[], int length) {
  char **cmds = malloc(length * sizeof(char *));
  int i = 0;
  for (char *pt = strtok(cmd, "|"); pt; pt = strtok(NULL, "|"), i++) {
    cmds[i] = malloc(strlen(pt) * sizeof(char));
    strcpy(cmds[i], pt);
  }
  return cmds;
}

int main(void) {
  char cmd_raw[] = "echo \"Hello 1\nHello 2\nNo 3\"|grep Hello|grep 1";
  int len = count_pipe_cmd(cmd_raw);
  char **cmds = split_cmds_new(cmd_raw, len);

  int fd[2];
  int in = STDIN_FILENO;
  pid_t pids[len];
  for (int i = 0; i < len - 1; ++i) {
    pipe(fd);
    if ((pids[i] = fork()) == 0) {
      run((char *)cmds[i], in, fd[1]);
    }
    close(fd[1]);
    in = fd[0];
  }
  run((char *)cmds[len - 1], in, STDOUT_FILENO);

  /* free cmds new */
  for (int i = 0; i < len; ++i)
    free(cmds[i]);
  free(cmds);
  return 0;
}
