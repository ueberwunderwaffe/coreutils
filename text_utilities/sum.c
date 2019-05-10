#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

#define BUFF_SIZE 4096

int set_flag(int, char **, int *, char *);
int print(int, char **, int, char);
int bsd_alg(char *, unsigned long int *, unsigned int *);
int sysv_alg(char *, unsigned long int *, unsigned int *);

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
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      unsigned long int checksum = 0;
      unsigned int blocks = 0;

      if (main_flag == 's') {
        int sysv_alg_error = sysv_alg(argv[i], &checksum, &blocks);
        if (sysv_alg_error == ERROR)
          continue;

        printf("%lu %d %s\n", checksum, blocks, argv[i]);
      } else {
        int bsd_alg_error = bsd_alg(argv[i], &checksum, &blocks);
        if (bsd_alg_error == ERROR)
          continue;

        if (many_files)
          printf("%lu %5d %s\n", checksum, blocks, argv[i]);
        else
          printf("%lu %5d\n", checksum, blocks);
      }
    }
  }

  return (TRUE);
}

int bsd_alg(char *file_name, unsigned long int *checksum,
            unsigned int *blocks) {
  FILE *file = NULL;
  file = fopen(file_name, "r");
  if (file == NULL) {
    printf("sum: %s: No such file or directory\n", file_name);
    return (ERROR);
  }

  char symbol;
  unsigned long int total_bytes = 0;

  while ((symbol = fgetc(file)) && !feof(file)) {
    total_bytes++;
    *checksum = (*checksum >> 1) + ((*checksum & 1) << 15);
    *checksum += symbol;
    *checksum &= 0xffff;
  }

  while (total_bytes > 0) {
    *blocks += 1;
    total_bytes /= 1024;
  }

  fclose(file);

  return (TRUE);
}

int sysv_alg(char *file_name, unsigned long int *checksum,
             unsigned int *blocks) {
  FILE *file = NULL;
  file = fopen(file_name, "r");
  if (file == NULL) {
    printf("sum: %s: No such file or directory\n", file_name);
    return (ERROR);
  }

  char buff[BUFF_SIZE];
  unsigned int text = 0;
  unsigned long int total_bytes = 0;

  while (fgets(buff, BUFF_SIZE, file) && !feof(file)) {
    int str_len = strlen(buff);

    for (int i = 0; i < str_len; ++i)
      text += buff[i];
    total_bytes += str_len;
  }

  int pointer = (text & 0xffff) + ((text & 0xffffffff) >> 16);
  *checksum = (pointer & 0xffff) + (pointer >> 16);

  while (total_bytes > 0) {
    *blocks += 1;
    total_bytes /= 512;
  }

  fclose(file);

  return (TRUE);
}
