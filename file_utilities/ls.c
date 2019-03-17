#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define YELLOW_COLOR "\e[33;1m"
#define BLUE_COLOR "\e[36;1m"
#define RESET_COLOR "\e[m"

int compare(const void *a, const void *b) {
  char *first = (char *)malloc(strlen(*(char **)a) * sizeof(char));
  char *second = (char *)malloc(strlen(*(char **)b) * sizeof(char));

  strcpy(first, *(char **)a);
  strcpy(second, *(char **)b);

  for (int i = 0; first[i] != '\0'; ++i)
    first[i] = tolower(first[i]);
  for (int i = 0; second[i] != '\0'; ++i)
    second[i] = tolower(second[i]);

  int result = strcmp(first, second);

  free(first);
  free(second);

  return (result);
}

int flags(int argc, char **argv, char **curr_file, int num_files, int *a_flag) {
  int error = FALSE;
  int f_flag = TRUE;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'f' && argv[i][2] == '\0') {
        *a_flag = TRUE;
        f_flag = FALSE;
      } else if (argv[i][1] == 'a' && argv[i][2] == '\0') {
        *a_flag = TRUE;
      }
    } else {
      error = TRUE;
      printf("ls: cannot access '%s': No such file or directory\n", argv[i]);
    }
  }

  if (f_flag) {
    // Sort in alphabetical order.
    qsort(curr_file, num_files, sizeof(const char **), compare);
  }

  return (error);
}

int main(int argc, char **argv) {
  char *curr_dir = NULL;
  char **curr_file = NULL;
  DIR *dir_pointer = NULL;
  struct dirent *dir_content = NULL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  curr_dir = getenv("PWD");
  if (curr_dir == NULL) {
    printf("[ERROR]: Couldn't get the directory\n");
    return (-1);
  }

  int num_files = 0;
  dir_pointer = opendir((const char *)curr_dir);
  while ((dir_content = readdir(dir_pointer)) != NULL)
    num_files++;
  closedir(dir_pointer);

  dir_pointer = NULL;
  dir_content = NULL;

  if (num_files == 0) {
    return (0);
  } else {
    curr_file = (char **)malloc(num_files * sizeof(char));
    for (int i = 0; i < num_files; i++)
      curr_file[i] = (char *)malloc(256 * sizeof(char));

    if (curr_file == NULL) {
      printf("[FAILED]: Memory allocation\n");

      for (int i = 0; i < num_files; i++)
        free(curr_file[i]);
      free(curr_file);

      return (-1);
    }
  }

  dir_pointer = opendir((const char *)curr_dir);
  if (dir_pointer == NULL) {
    printf("[ERROR]: Couldn't open the directory");
    free(curr_file);
    return (-1);
  }

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL; ++i) {
    curr_file[i] = dir_content->d_name;
  }

  int a_flag = FALSE;
  int flag_error = flags(argc, argv, curr_file, num_files, &a_flag);
  if (flag_error) {
    closedir(dir_pointer);
    free(curr_file);
    return (-1);
  }

  for (int i = 0; i < num_files; ++i) {
    struct stat st;

    if (curr_file[i][0] != '.' || a_flag) {
      // Check if the file is executable
      if (stat((const char *)curr_file[i], &st) == 0 && st.st_mode & S_IXUSR) {
        int fd = -1;

        fd = open((const char *)curr_file[i], O_RDONLY, 0);
        if (fd == -1) {
          printf("[FAILED]: Opening file/directory\n");
          closedir(dir_pointer);
          free(curr_file);
          return (-1);
        }

        fstat(fd, &st);
        if (S_ISDIR(st.st_mode)) {
          printf(BLUE_COLOR "%s  " RESET_COLOR, (const char *)curr_file[i]);
        } else {
          printf(YELLOW_COLOR "%s  " RESET_COLOR, (const char *)curr_file[i]);
        }
        close(fd);
      } else {
        printf("%s  ", (const char *)curr_file[i]);
      }
    }
  }
  putchar('\n');

  closedir(dir_pointer);
  free(curr_file);

  return (0);
}
