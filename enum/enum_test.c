#include <stdio.h>
#include <string.h>

/*! \enum AccessMode
 *
 *  Detailed description
 */
enum AccessMode {
  MemByteAccess,
  MemWordAccess,
  MemDWordAccess,
};

void check_mode(enum AccessMode e) {
  void *p_test;
  int a = 65;
  p_test = &a;

  printf("cast to int addr: %p\n", (int *)p_test);
  printf("cast to char addr: %p\n", (char *)p_test);
  printf("cast to int: %d\n", *(int *)p_test);
  printf("cast to char: %c\n", *(char *)p_test);
  printf("sizeof cast int: %lu\n", sizeof *(int *)p_test);
  printf("sizeof cast char: %lu\n", sizeof *(char *)p_test);

  printf("next int addr: %p\n", (unsigned int *)p_test + 1);
  printf("next char addr: %p\n", (unsigned char *)p_test + 1);
  printf("cast to int: %d\n", *(int *)(p_test + 1));
  printf("cast to char: %c\n", *(char *)(p_test + 1));

  switch (e) {
  case MemByteAccess:
    printf("MemByteAccess\n");
    break;
  case MemWordAccess:
    printf("MemWordAccess\n");
    break;
  case MemDWordAccess:
    printf("MemDWordAccess\n");
    break;
  default:
    printf("default\n");
  }
}

int main(int argc, char *argv[]) {
  check_mode(MemWordAccess);
  const char *const a = "22123";
  /* printf("%lu\n", strlen(a)); */
  for (int i = 0; i < strlen(a); ++i) {
    printf("%c\n", a[i]);
  }
  printf("sizeof NULL: %lu\n", sizeof NULL);
  printf("sizeof int: %lu\n", sizeof(int));

  return 0;
}
