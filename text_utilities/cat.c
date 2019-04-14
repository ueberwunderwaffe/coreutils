#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

#define SIZE 2048

struct command_flags {
  int b_flag;
  int E_flag;
  int n_flag;
  int T_flag;
} flags;

int number_analyzer(char *number);
int set_flags(int, char **);
int print(int, char **);

int main(int argc, char **argv) {
  flags.b_flag = FALSE;
  flags.E_flag = FALSE;
  flags.n_flag = FALSE;
  flags.T_flag = FALSE;

  int flag_error = set_flags(argc, argv);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  int print_error = print(argc, argv);
  if (print_error == ERROR) {
    printf("[ERROR main()]: print()\n");
    return (ERROR);
  }

  return (0);
}

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

int set_flags(int argc, char **argv) {
  for (int i = argc - 1; i > 0; --i) {
    if (argv[i][0] == '-') {
      if ((argv[i][1] == 'b' && argv[i][2] == '\0') ||
          strcmp(argv[i], "--number-nonblank") == 0) {
        flags.b_flag = TRUE;
      } else if ((argv[i][1] == 'E' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--show-ends") == 0) {
        flags.E_flag = TRUE;
      } else if ((argv[i][1] == 'n' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--number") == 0) {
        flags.n_flag = TRUE;
      } else if ((argv[i][1] == 'T' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--show-tabs") == 0) {
        flags.T_flag = TRUE;
      } else {
        printf("cat: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'cat --help' for more information.");
        return (ERROR);
      }
    }
  }

  return (TRUE);
}

int print(int argc, char **argv) {
  FILE *file;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      int num_analyzer_error = number_analyzer(argv[i]);
      if (num_analyzer_error == ERROR) {
        file = NULL;
        file = fopen(argv[i], "r");
        if (file == NULL) {
          printf("cat: %s: No such file or directory\n", argv[i]);
          continue;
        }

        char line[SIZE];
        char end = '\0';
        char tmp1, tmp2;

        for (int i = 1; fgets(line, SIZE, file) && !feof(file); ++i) {
          if (flags.T_flag) {
            for (int j = 0; line[j] != '\0'; ++j) {
              if (line[j] == '\t') {
                line[j] = '^';
                j++;
                tmp1 = line[j];
                line[j] = 'I';
                for (int k = j + 1; line[k - 1] != '\0'; ++k) {
                  tmp2 = line[k];
                  line[k] = tmp1;
                  tmp1 = tmp2;
                }
              }
            }
          }

          if (flags.E_flag) {
            for (int j = 0; line[j] != '\0'; ++j) {
              if (line[j] == '\n') {
                line[j] = '$';
                end = '\n';
              }
            }
          }

          if (flags.n_flag || flags.b_flag) {
            if (flags.b_flag && (line[0] == '\n' || line[0] == '$')) {
              printf("%s%c", line, end);
              i--;
            } else {
              printf("%*d  %s%c", 6, i, line, end);
            }
          } else {
            printf("%s%c", line, end);
          }
        }

        fclose(file);
      }
    }
  }

  return (TRUE);
}
