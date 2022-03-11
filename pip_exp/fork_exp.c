#include <stdio.h>
#include <unistd.h>

int main(void)
{
  pid_t pid;
  if ((pid = fork()) == 0) {
    printf("Hello world\n");
  }
  printf("1: getpid: %d, pid: %d\n", getpid(), pid);

  if ((pid = fork()) == 0) {
    printf("Hello world\n");
  }
  printf("2: getpid: %d, pid: %d\n", getpid(), pid);
  return 0;
}

/* pid: 380480 -> parent, first printf
 * pid: 380481 -> parent, second printf
 * Hello world -> child 1, first if
 * pid: 0      -> child 1, first printf
 * Hello world -> child 2
 * pid: 380482 -> child 1
 * pid: 0      -> child 2
 * Hello world -> child 2
 * pid: 0      -> child 2
 * */

/* 1: getpid: 382562, pid: 382563
 * 2: getpid: 382562, pid: 382564
 * Hello world
 * Hello world
 * 1: getpid: 382563, pid: 0
 * 2: getpid: 382564, pid: 0
 * 2: getpid: 382563, pid: 382565
 * Hello world
 * 2: getpid: 382565, pid: 0 */
