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

int main() {
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
    if (dir_content->d_name[0] != '.')
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
      
      for(int i = 0; i < num_files; i++)
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

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL;) {
    if (dir_content->d_name[0] != '.') {
      curr_file[i] = dir_content->d_name;
      ++i;
    }
  }
  
  // Sort in alphabetical order.
  qsort(curr_file, num_files, sizeof(const char **), compare);

  for (int i = 0; i < num_files; ++i) {
    struct stat st;
    // Check if the file is executable
    if (stat((const char *)curr_file[i], &st) == 0 && st.st_mode & S_IXUSR) {
      int fd = -1;

      fd = open((const char *)curr_file[i], O_RDONLY, 0);
      if (fd == -1) {
        printf("[FAILED]: Opening file/directory\n");
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
  putchar('\n');

  closedir(dir_pointer);
  free(curr_file);

  return (0);
}
