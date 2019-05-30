#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int c_flag;
  int m_flag;
  int l_flag;
  int w_flag;
} flags;

int set_flags(int, char **, int *);
int print(int, char **, int);
int column_max_width(unsigned long long int);

int main(int argc, char **argv) {
  int many_files = FALSE;

  flags.c_flag = FALSE;
  flags.m_flag = FALSE;
  flags.l_flag = FALSE;
  flags.w_flag = FALSE;

  int flag_error = set_flags(argc, argv, &many_files);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  int print_error = print(argc, argv, many_files);
  if (print_error == ERROR) {
    printf("[ERROR main()]: print()\n");
    return (ERROR);
  }

  return (0);
}

int set_flags(int argc, char **argv, int *many_files) {
  int many_arguments = FALSE;
  for (int i = argc - 1; i > 0; --i) {
    if (argv[i][0] == '-') {
      if ((argv[i][1] == 'c' && argv[i][2] == '\0') ||
          strcmp(argv[i], "--bytes") == 0) {

      } else if ((argv[i][1] == 'm' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--chars") == 0) {

      } else if ((argv[i][1] == 'l' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--lines") == 0) {

      } else if ((argv[i][1] == 'w' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--words") == 0) {

      } else {
        printf("wc: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'wc --help' for more information.\n");
        return (ERROR);
      }
    }

    if (many_arguments)
      *many_files = TRUE;
    else
      many_arguments = TRUE;
  }

  return (TRUE);
}

int print(int argc, char **argv, int many_files) {
  FILE *file;

  unsigned long long int total_bytes = 0;
  unsigned long long int total_words = 0;
  unsigned long long int total_lines = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      unsigned long long int bytes = 0;
      unsigned long long int words = 0;
      unsigned long long int lines = 0;

      file = NULL;
      file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("wc: %s: No such file or directory\n", argv[i]);
        continue;
      }

      char symbol;
      char early = ' ';
      while ((symbol = fgetc(file)) && !feof(file)) {
        if ((symbol == ' ' && early != ' ' && early != '\n') ||
            (symbol == '\n' && early != '\n'))
          words++;

        if (symbol == '\n' || symbol == '\0')
          lines++;

        bytes++;
        early = symbol;
      }

      if (many_files) {
        total_bytes += bytes;
        total_words += words;
        total_lines += lines;
      }

      int width = column_max_width(bytes);
      printf("%*llu", width, lines);
      printf(" %*llu", width, words);
      printf(" %*llu %s\n", width, bytes, argv[i]);

      fclose(file);
    }
  }

  int width = column_max_width(total_bytes);
  if (many_files) {
    printf("%*llu", width, total_lines);
    printf(" %*llu", width, total_words);
    printf(" %*llu %s\n", width, total_bytes, "total");
  }

  return (TRUE);
}

int column_max_width(unsigned long long int value) {
  int result = 0;

  while (value > 0) {
    value /= 10;
    result++;
  }

  return result;
}
