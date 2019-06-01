#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

#define SIZE 256

struct command_flags {
  int L_flag;
  int P_flag;
} flags;

int set_flags(int, char **);
int print();

int main(int argc, char **argv) {
  flags.L_flag = FALSE;
  flags.P_flag = FALSE;

  int flag_error = set_flags(argc, argv);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  } else if (flag_error == FALSE) {
    return (0);
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
      if ((argv[i][1] == 'L' && argv[i][2] == '\0') ||
          strcmp(argv[i], "--logical") == 0) {
        flags.L_flag = TRUE;
      } else if ((argv[i][1] == 'P' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--physical") == 0) {
        flags.P_flag = TRUE;
      } else {
        printf("pwd: bad option: -%c\n", argv[i][1]);
        return (FALSE);
      }
    } else {
      printf("pwd: too many arguments\n");
      return (FALSE);
    }
  }

  return (TRUE);
}

int print() {
  char *curr_dir = NULL;
  if (flags.P_flag == TRUE) {
    curr_dir = (char *)malloc(SIZE * sizeof(char));
    if (curr_dir == NULL) {
      printf("[FAILED print()]: Memory allocation\n");
      return (ERROR);
    }

    getcwd(curr_dir, SIZE);
    if (curr_dir == NULL) {
      printf("[ERROR print()]: Couldn't get the directory\n");
      return (ERROR);
    }
    printf("%s\n", curr_dir);
  } else {
    curr_dir = getenv("PWD");
    if (curr_dir == NULL) {
      printf("[ERROR print()]: Couldn't get the directory\n");
      return (ERROR);
    }
    printf("%s\n", curr_dir);
  }

  return (TRUE);
}
