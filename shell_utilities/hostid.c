#include <stdio.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int main(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        printf("hostid: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'hostid --help' for more information.\n");
        return (ERROR);
      }
    }

    printf("hostid: extra operand ’%s’\n", argv[1]);
    printf("Try 'hostid --help' for more information.\n");
    return (ERROR);
  } else {
    unsigned int id = gethostid();
    id &= 0xffffffff;
    printf("%08x\n", id);
  }

  return (0);
}
