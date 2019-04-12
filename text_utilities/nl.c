#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

#define SIZE 2048

struct command_flags {
  int i_flag;
  int v_flag;
  int w_flag;
} flags;

int number_analyzer(char *number);
int set_flags(int, char **, int *, int *, int *);
int print(int, char **, int, int, int);

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

  int print_error = print(argc, argv, line_inc, line_start, num_width);
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

int set_flags(int argc, char **argv, int *line_inc, int *line_start,
              int *num_width) {
  for (int i = argc - 1; i > 0; --i) {
    if (argv[i - 1][0] == '-') {
      if ((argv[i - 1][1] == 'i' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--line-increment") == 0) {
        flags.i_flag = TRUE;
        *line_inc = number_analyzer(argv[i]);
        if(*line_inc == ERROR) {
          printf("nl: invalid line number increment: ’%s’\n", argv[i]);
          return (ERROR);
        }
      } else if ((argv[i - 1][1] == 'v' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--starting-line-number") == 0) {
        flags.v_flag = TRUE;
        *line_start = number_analyzer(argv[i]);
        if(*line_start == ERROR) {
          printf("nl: invalid starting line number: ’%s’\n", argv[i]);
          return (ERROR);
        }
      } else if ((argv[i - 1][1] == 'w' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--number-width") == 0) {
        flags.w_flag = TRUE;
        *num_width = number_analyzer(argv[i]);
        if(*num_width == ERROR) {
          printf("nl: invalid line number field width: ’%s’\n", argv[i]);
          return (ERROR);
        }
      } else {
        printf("nl: invalid option -- '%c'\n", argv[i - 1][1]);
        printf("Try 'nl --help' for more information\n");
        return (ERROR);
      }
    }
  }

  return (TRUE);
}

int print(int argc, char **argv, int line_inc, int line_start, int num_width) {
  FILE *file;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      int num_analyzer_error = number_analyzer(argv[i]);
      if (num_analyzer_error == ERROR) {
        file = NULL;
        file = fopen(argv[i], "r");
        if (file == NULL) {
          printf(
              "nl: cannot open '%s' for reading: No such file or directory\n",
              argv[i]);
          continue;
        }

        char line[SIZE];
        for (; fgets(line, SIZE, file) && !feof(file); line_start += line_inc) {
          printf("%*d  %s", num_width, line_start, line);
        }

        fclose(file);
      }
    }
  }

  return (TRUE);
}
