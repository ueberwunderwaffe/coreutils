#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int i_flag;
  int t_flag;
} flags;

int number_analyzer(char *);
int set_flags(int, char **, int *);

int main(int argc, char **argv) {
  int tabs = 4;

  flags.i_flag = FALSE;
  flags.t_flag = FALSE;

  int flag_error = set_flags(argc, argv, &tabs);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  return (0);
}

int number_analyzer(char *number) {
  int length = strlen(number);
  int dec = 1;
  int result = 0;

  for (int i = length - 1; i >= 0; --i) {
    if (number[i] > 47 && (int)number[i] < 58) {
      result += (number[i] - '0') * dec;
      dec *= 10;
    } else {
      return (ERROR);
    }
  }

  return (result);
}

int set_flags(int argc, char **argv, int *tabs) {
  for (int i = argc; i > 1; --i) {
    if (argv[i - 1][0] == '-') {
      if ((argv[i - 1][1] == 'i' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--initial") == 0) {
        flags.i_flag = TRUE;
      } else if ((argv[i - 1][1] == 't' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--tabs") == 0) {
        *tabs = number_analyzer(argv[i]);
        if (*tabs == ERROR) {
          printf("expand: tab size contains invalid character(s): ‘%s‘\n",
                 argv[i]);
          return (ERROR);
        }
        flags.t_flag = TRUE;
      }
    }
  }

  return (TRUE);
}
