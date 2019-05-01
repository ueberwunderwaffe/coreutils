#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int c_flag;
  int n_flag;
  int q_flag;
  int v_flag;
  int z_flag;
} flags;

int number_analyzer(char *);
int set_flags(int, char **, int *, int *, int *, char *);
int print(int, char **, int, int, int, char);

int main(int argc, char **argv) {
  int num_bytes = 0;  // -c
  int num_lines = 10; // -n
  int many_files = FALSE;
  char main_flag = ' ';

  flags.c_flag = FALSE;
  flags.n_flag = FALSE;
  flags.z_flag = FALSE;

  int flag_error =
      set_flags(argc, argv, &num_bytes, &num_lines, &many_files, &main_flag);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  int print_error =
      print(argc, argv, num_bytes, num_lines, many_files, main_flag);
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
    if (number[i] > 47 && (int)number[i] < 58) {
      result += (number[i] - '0') * dec;
      dec *= 10;
    } else {
      return (ERROR);
    }
  }

  return (result);
}

int set_flags(int argc, char **argv, int *num_bytes, int *num_lines,
              int *many_files, char *main_flag) {
  int many_arguments = FALSE;
  for (int i = argc; i > 1; --i) {
    if (argv[i - 1][0] == '-') {
      if (i == argc && argv[i - 1][1] != 'z') {
        printf("tail: option requires an argument -- '%c'\n", argv[i - 1][1]);
        printf("Try 'tail --help' for more information.\n");
        return (ERROR);
      }

      if ((argv[i - 1][1] == 'c' && argv[i - 1][2] == '\0') ||
          strcmp(argv[i - 1], "--bytes") == 0) {
        if (*main_flag == ' ') {
          *num_bytes = number_analyzer(argv[i]);
          if (*num_bytes == ERROR) {
            printf("head: invalid number of bytes: ‘%s‘\n", argv[i]);
            return (ERROR);
          }

          *main_flag = 'c';
        }

        flags.c_flag = TRUE;
        --i;
      } else if ((argv[i - 1][1] == 'n' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--lines") == 0) {
        if (*main_flag == ' ') {
          *num_lines = number_analyzer(argv[i]);
          if (*num_lines == ERROR) {
            printf("head: invalid number of lines: ‘%s‘\n", argv[i]);
            return (ERROR);
          }

          *main_flag = 'n';
        }

        flags.n_flag = TRUE;
        --i;
      } else if ((argv[i - 1][1] == 'q' && argv[i - 1][2] == '\0') ||
                 (strcmp(argv[i - 1], "--quiet") == 0) ||
                 (strcmp(argv[i - 1], "--silent") == 0)) {
        flags.q_flag = TRUE;
        --i;
      } else if ((argv[i - 1][1] == 'v' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--verbose") == 0) {
        flags.v_flag = TRUE;
        --i;
      } else if ((argv[i - 1][1] == 'z' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--zero-terminated") == 0) {
        if (*main_flag == ' ')
          *main_flag = 'z';
        flags.z_flag = TRUE;
      }
    } else {
      if (many_arguments)
        *many_files = TRUE;
      else
        many_arguments = TRUE;
    }
  }

  return (TRUE);
}

int output_start(FILE *file, int size, int *num_lines) {
  fseek(file, 0, SEEK_END);
  int cur_line = 0;
  char symbol;

  for (int i = 0; i < size; i++) {
    fseek(file, size - 2 - i, SEEK_SET);
    symbol = fgetc(file);
    if (symbol == '\n') {
      cur_line++;
      if (cur_line == *num_lines)
        return (size - 1 - i);
    }
  }

  return (ERROR);
}

int print(int argc, char **argv, int num_bytes, int num_lines, int many_files,
          char main_flag) {
  FILE *file;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      int num_analyzer_error = number_analyzer(argv[i]);
      if (num_analyzer_error == ERROR) {
        int bytes = num_bytes;
        int lines = num_lines;
        file = NULL;
        file = fopen(argv[i], "r");
        if (file == NULL) {
          printf(
              "head: cannot open '%s' for reading: No such file or directory\n",
              argv[i]);
          continue;
        }

        if ((many_files && flags.q_flag != TRUE) || flags.v_flag)
          printf("==> %s <==\n", argv[i]);

        fseek(file, 0, SEEK_END);
        int size = ftell(file);

        int start = output_start(file, size, &num_lines);
        if (start == ERROR) {
          printf("[ERROR print()]: Cann't determine the beginning of the "
                 "reading.\n");
          return (ERROR);
        }

        fseek(file, start, SEEK_SET);

        char symbol;
        if (main_flag == 'c') {
          while (((symbol = fgetc(file)) && !feof(file)) && bytes > 0 &&
                 start < size) {
            --bytes;
            start++;
            putchar(symbol);
          }
        } else if (main_flag == 'z') {
          while (((symbol = fgetc(file)) && !feof(file)) && start < size) {
            putchar(symbol);
            start++;
          }
        } else {
          while (((symbol = fgetc(file)) && !feof(file)) && lines > 0 &&
                 start < size) {
            if (symbol == '\n')
              --lines;
            putchar(symbol);
            start++;
          }
        }

        if (many_files && i != argc - 1)
          putchar('\n');

        fclose(file);
      }
    }
  }

  return (TRUE);
}
