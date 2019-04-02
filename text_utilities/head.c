#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

struct command_flags {
  int c_flag;
  int n_flag;
  int z_flag;
} flags;

int set_flags(int, char **, int *);
int print(int, char **, int, int *);

int set_flags(int argc, char **argv, int *many_files) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if ((argv[i][1] == 'c' && argv[i][2] == '\0') ||
          strcmp(argv[i], "--bytes")) {
        flags.c_flag = TRUE;
      } else if ((argv[i][1] == 'n' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--lines")) {
        flags.n_flag = TRUE;
      } else if ((argv[i][1] == 'z' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--zero-terminated")) {
        flags.z_flag = TRUE;
      }
    } else {
      *many_files = TRUE;
    }
  }

  return (TRUE);
}

int print(int argc, char **argv, int num_lines, int *many_files) {
  FILE * file;
 
  for(int i = 1; i < argc; ++i) {
    if(argv[i][0] != '-') {
      int lines = num_lines;
      file = NULL;
      file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("head: cannot open '%s' for reading: No such file or directory\n", argv[i]);
        continue;
      }

      if(*many_files)
        printf("==> %s <==\n", argv[i]);

      char symbol;
      while((symbol = fgetc(file)) &&!feof(file) && lines > 0) {
        if (symbol == '\n')
          --lines;
        putchar(symbol);
      }

      if(*many_files && i != argc - 1)
        putchar('\n');

      fclose(file);
    }
  }

  return (TRUE);
}

int main(int argc, char **argv) {
  int num_lines = 10;
  int many_files = FALSE;

  flags.c_flag = FALSE;
  flags.n_flag = FALSE;
  flags.z_flag = FALSE;

  int flag_error = set_flags(argc, argv, &many_files);
  if (flag_error == ERROR) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    return (ERROR);
  }

  int print_error = print(argc, argv, num_lines, &many_files);
  if (print_error == ERROR) {
    printf("[ERROR main()]: print()\n");
    return (ERROR);
  }

  return (0);
}
