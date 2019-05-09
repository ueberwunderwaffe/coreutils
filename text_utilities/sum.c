#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int set_flag(int, char **, int *, char *);
int print(int, char **, int, char);

int main(int argc, char **argv) {
  int many_files = FALSE;
  char main_flag = ' ';

  int flag_error = set_flag(argc, argv, &many_files, &main_flag);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flag\n");
    return (ERROR);
  }

  int print_error = print(argc, argv, many_files, main_flag);
  if (print_error == ERROR) {
    printf("[ERROR main()]: print()\n");
    return (ERROR);
  }

  return (0);
}

int set_flag(int argc, char **argv, int *many_files, char *main_flag) {
  int many_arguments = FALSE;

  for (int i = argc; i > 1; --i) {
    if (argv[i - 1][0] == '-') {
      if (argv[i - 1][1] == 'r' && argv[i - 1][2] == '\0') {
        if (*main_flag == ' ')
          *main_flag = 'r';
      } else if ((argv[i - 1][1] == 's' && argv[i - 1][2] == '\0') ||
                 strcmp(argv[i - 1], "--sysv") == 0) {
        if (*main_flag == ' ')
          *main_flag = 's';
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

int print(int argc, char **argv, int many_files, char main_flag) {
  FILE *file;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      file = NULL;
      file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("sum: %s: No such file or directory\n", argv[i]);
        continue;
      }

      unsigned long int checksum = 0;
      unsigned int blocks = 0;

      fclose(file);
    }
  }

  return (TRUE);
}
