#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int a_flag;
  int t_flag;
} flags;

int number_analyzer(char *);
int set_flags(int, char **, int *);
void print(int, char **, int);

int main(int argc, char **argv) {
  int tabs = 8;

  flags.a_flag = FALSE;
  flags.t_flag = FALSE;

  int flag_error = set_flags(argc, argv, &tabs);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  print(argc, argv, tabs);

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
      if ((argv[i - 1][1] == 'a' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--all") == 0) {
        flags.a_flag = TRUE;
      } else if ((argv[i - 1][1] == 't' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--tabs") == 0) {
        *tabs = number_analyzer(argv[i]);
        if (*tabs == ERROR) {
          printf("unexpand: tab size contains invalid character(s): ‘%s‘\n",
                 argv[i]);
          return (ERROR);
        }
        flags.t_flag = TRUE;
      }
    }
  }

  return (TRUE);
}

void print(int argc, char **argv, int tabs) {
  FILE *file;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      int num_analyzer_error = number_analyzer(argv[i]);
      if (num_analyzer_error == ERROR) {
        file = NULL;
        file = fopen(argv[i], "r");
        if (file == NULL) {
          printf("unexpand: %s: No such file or directory\n", argv[i]);
          continue;
        }

        char symbol;
        int blanks = 0;
        int initial_blank = TRUE;

        while ((symbol = fgetc(file)) && !feof(file)) {
          if (symbol == ' ' && (flags.a_flag || initial_blank == TRUE)) {
            ++blanks;
            if (blanks == tabs) {
              putchar('\t');
              blanks = 0;
            }
          } else {
            if (blanks != 0)
              for (int i = 0; i < blanks; ++i)
                putchar(' ');

            putchar(symbol);
            blanks = 0;

            if (symbol == '\n')
              initial_blank = TRUE;
            else
              initial_blank = FALSE;
          }
        }

        fclose(file);
      }
    }
  }
}
