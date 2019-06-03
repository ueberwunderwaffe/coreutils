#include <stdio.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int main(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        printf("logname: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'logname --help' for more information.\n");
        return (ERROR);
      }
    }

    printf("logname: extra operand ’%s’\n", argv[1]);
    printf("Try 'logname --help' for more information.\n");
    return (ERROR);
  } else {
    char *name = getlogin();

    if (name != NULL) {
      printf("%s\n", name);
    } else {
      printf("logname: no login name\n");
      return (ERROR);
    }
  }

  return (0);
}
