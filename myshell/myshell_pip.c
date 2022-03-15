#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int gMaxLineLength = 256;

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

int run_process(char *cmds[], int len) {
  int status;
  int fd[2];
  int in = STDIN_FILENO;
  pid_t pids[len];

  for (int i = 0; i < len; ++i) {
    /* last command */
    if (i == len - 1) {
      if ((pids[i] = fork()) == 0)
        run(cmds[i], in, STDOUT_FILENO);
      break;
    }
    /* create pipe */
    pipe(fd);
    if ((pids[i] = fork()) == 0) {
      run(cmds[i], in, fd[1]);
    }
    close(fd[1]);
    in = fd[0];
  }

  /* wait for all commad */
  for (int i = 0; i < len; ++i) {
    int status;
    waitpid(pids[i], &status, WUNTRACED);
  }
  return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
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

int check_cmds(char *cmds[], int length) {
  for (int i = 0; i < length; ++i) {
    if (cmds[i] == NULL) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  char raw_cmd[gMaxLineLength];
  while (1) {
    printf("ᕕ( ᐛ )ᕗ ");
    fgets(raw_cmd, sizeof(raw_cmd), stdin);
    if (feof(stdin)) {
      break;
    }
    raw_cmd[strlen(raw_cmd) - 1] = '\0';

    /* count if pipe
     *    1: one command
     *    2, 3, ....: # command */
    int len = count_pipe_cmd(raw_cmd);
    char **cmds = split_cmds_new(raw_cmd, len);
    if (check_cmds(cmds, len)) {
      printf("Error: pipe to empty command\n");
      continue;
    }
    run_process(cmds, len);
    for (int i = 0; i < len; ++i)
      free(cmds[i]);
    free(cmds);
  }

  printf("\n");
  return 0;
}
