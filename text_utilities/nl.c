#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int i_flag;
  int v_flag;
  int w_flag;
} flags;

int set_flags(int, char **, int *, int *, int *);

int number_analyzer(char *number) {
  int length = strlen(number);
  int dec = 1;
  int result = 0;

  for (int i = length - 1; i >= 0; --i) {
    if (number[i] > 47 && number[i] < 58) {
      result += (number[i] - '0') * dec;
      dec *= 10;
    } else {
      return (ERROR);
    }
  }

  return (result);
}

int set_flags(int argc, char **argv, int *line_inc, int *line_start,
              int *num_width) {
  for (int i = argc; i > 1; --i) {
    if (argv[i - 1][0] == '-') {
      if ((argv[i - 1][1] == 'i' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--line-increment")) {
        flags.i_flag = TRUE;
        *line_inc = number_analyzer(argv[i]);
      } else if ((argv[i - 1][1] == 'v' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--starting-line-number")) {
        flags.v_flag = TRUE;
        *line_start = number_analyzer(argv[i]);
      } else if ((argv[i - 1][1] == 'w' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--number-width")) {
        flags.w_flag = TRUE;
        *num_width = number_analyzer(argv[i]);
      } else {
        printf("nl: invalid option -- '%c'\n", argv[i - 1][1]);
        printf("Try 'nl --help' for more information\n");
        return (ERROR);
      }
    }
  }

  return (TRUE);
}

int main(int argc, char **argv) {
  int line_inc = 1;
  int line_start = 1;
  int num_width = 6;

  flags.i_flag = FALSE;
  flags.v_flag = FALSE;
  flags.w_flag = FALSE;

  int flag_error = set_flags(argc, argv, &line_inc, &line_start, &num_width);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  return (0);
}
