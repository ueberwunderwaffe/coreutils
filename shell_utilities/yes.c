#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int main(int argc, char **argv) {
  if (argc > 1) {
    if (argv[1][0] == '-') {
      printf("yes: invalid option -- '%c'\n", argv[1][1]);
      printf("Try 'yes -- help' for more information.\n");
      return (ERROR);
    } else {
      while (TRUE) {
        for (int i = 1; i < argc - 1; ++i)
          printf("%s ", argv[i]);
        printf("%s\n", argv[argc - 1]);
      }
    }
  } else {
    while (TRUE)
      printf("y\n");
  }

  return (0);
}
