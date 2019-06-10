#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int s_flag;
} flags;

int set_flags(int, char **);
int print();

int main(int argc, char **argv) {
  flags.s_flag = FALSE;

  int flag_error = set_flags(argc, argv);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  int print_error = print();
  if (print_error == ERROR) {
    printf("[ERROR main()]: print()\n");
    return (ERROR);
  }

  return (0);
}

int set_flags(int argc, char **argv) {
  for (int i = argc - 1; i > 0; --i) {
    if (argv[i][0] == '-') {
      if ((argv[i][1] == 's' && argv[i][2] == '\0') ||
          strcmp(argv[i], "--silent") == 0) {
        flags.s_flag = TRUE;
      } else {
        printf("tty: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'tty --help' for more information.\n");
        return (ERROR);
      }
    } else {
      printf("tty: extra operand ’%s’\n", argv[i]);
      printf("Try 'tty --help' for more information.\n");
      return (ERROR);
    }
  }

  return (TRUE);
}

int print() {
  char *tty = NULL;
  tty = ttyname(STDIN_FILENO);
  if (tty == NULL) {
    printf("[FAILED print()]: ttyname()\n");
    return (ERROR);
  }

  if (!flags.s_flag)
    printf("%s\n", tty);

  return (TRUE);
}
