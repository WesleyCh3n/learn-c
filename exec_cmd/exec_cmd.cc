#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

using std::string;

int c_execvp(char *args[]) {
  /* int *shared_mem = (int *)mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE,
   *                               MAP_SHARED | MAP_ANONYMOUS, -1, 0); */
  pid_t pid = fork(); // return child pid
  if (pid == 0) {
    printf("[CHILD] pid: %d\n", getpid());
    execvp(args[0], args);
    // child gets here only if there was an error
    perror("[CHILD] Error");
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("fork failed!");
  }

  printf("[PARENT] pid: %d\n", getpid());
  printf("[PARNET] get child pid: %d\n", pid);
  int status;
  waitpid(pid, &status, WUNTRACED);

  printf("[PARENT] WIFEXITED(childstate) %d\n", WIFEXITED(status));
  printf("[PARENT] WEXITSTATUS(childstate) %d\n", WEXITSTATUS(status));
  printf("[PARENT] WTERMSIG(childstate) %d\n", WTERMSIG(status));

  return WIFEXITED(status) ? WEXITSTATUS(status) : -WTERMSIG(status);
}

string c_popen(const char *cmd) {
  char buffer[128];
  string result;

  std::FILE *pipe = popen(cmd, "r");

  if (!pipe) {
    throw std::runtime_error("popen failed");
  }

  try {
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
      result += buffer;
    }
  } catch (std::exception &e) {
    pclose(pipe);
    throw e;
  }

  printf("status %d\n", WEXITSTATUS(pclose(pipe)));
  return result;
}

int main() {
  /* try {
   *   printf("%s", c_popen("touch foo 2>&1").c_str());
   *   printf("%s", c_popen("rm foo 2>&1").c_str());
   * } catch (std::exception &e) {
   *   printf("%s\n", e.what());
   * } */
  {
    const char *args[] = {"ls", "-l", nullptr};
    c_execvp((char **)args);
  }
  printf("\n");
  {
    const char *args[] = {"lsa", "-l", nullptr};
    c_execvp((char **)args);
  }
  return 0;
}
