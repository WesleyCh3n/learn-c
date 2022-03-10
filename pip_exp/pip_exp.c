#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int fds[2];
  pipe(fds);
  pid_t pid = fork();
  if (pid == 0) {
    close(fds[1]);
    char buf[6];
    read(fds[0], buf, sizeof(buf));
    printf("Read from pipe bridge process: %s\n", buf);
    close(fds[0]);
    return 0;
  }
  close(fds[0]);
  write(fds[1], "wesley", 6);
  waitpid(pid, NULL, 0);
  close(fds[1]);
  printf("%d %d\n", fds[0], fds[1]);
  return 0;
}
