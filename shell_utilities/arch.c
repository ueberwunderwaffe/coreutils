#include <stdio.h>
#include <sys/utsname.h>

#define ERROR -1

int main(int argc, char **argv) {
  struct utsname utsbuf;
  if (argc > 1) {
    if (argv[1][0] == '-') {
      printf("arch: invalid option -- '%c'\n", argv[1][1]);
      printf("Try 'arch --help' for more information.\n");
      return (ERROR);
    } else {
      printf("arch: extra operand ’%s’\n", argv[1]);
      printf("Try 'arch --help' for more information.\n");
      return (ERROR);
    }
  } else {
    if (uname(&utsbuf) != -1) {
      printf("%s\n", utsbuf.machine);
    } else {
      printf("[ERROR]: Some problems with the sys/utsname.h\n");
      return (ERROR);
    }
  }

  return (0);
}
