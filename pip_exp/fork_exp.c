#include <stdio.h>
#include <unistd.h>

int main(void)
{
  pid_t pid;
  if ((pid = fork()) == 0) {
    printf("1: getpid: %d, Hello inside\n", getpid());
  }
  printf("1: getpid: %d, pid: %d\n", getpid(), pid);

  if ((pid = fork()) == 0) {
    printf("2: getpid: %d, Hello inside\n", getpid());
  }
  printf("2: getpid: %d, pid: %d\n", getpid(), pid);
  return 0;
}

/* 1: getpid: 383422, pid: 383423
 * 2: getpid: 383422, pid: 383424
 * 1: getpid: 383423, Hello inside
 * 1: getpid: 383423, pid: 0
 * 2: getpid: 383424, Hello inside
 * 2: getpid: 383424, pid: 0
 * 2: getpid: 383423, pid: 383425
 * 2: getpid: 383425, Hello inside
 * 2: getpid: 383425, pid: 0 */
