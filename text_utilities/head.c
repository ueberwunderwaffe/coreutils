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

int set_flags(int argc, char **argv, int *many_files) {
  for (int i = 1; i < argc; i++) {
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
      printf("head: cannot open '%s' for reading: No such file or directory\n",
             argv[i]);
      *many_files = TRUE;
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

  printf("many_files: %d\n", many_files);

  return (0);
}
