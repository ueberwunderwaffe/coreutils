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

int main() {
  char *curr_dir = NULL;
  long *curr_file = NULL;
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
    curr_file = (long *)malloc(num_files * sizeof(long));
    if (curr_file == NULL) {
      printf("[FAILED]: Memory allocation\n");
      free(curr_file);
      return (-1);
    }
    memset(curr_file, 0, num_files * sizeof(long));
  }

  dir_pointer = opendir((const char *)curr_dir);
  if (dir_pointer == NULL) {
    printf("[ERROR]: Couldn't open the directory");
    free(curr_file);
    return (-1);
  }

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL;) {
    if (dir_content->d_name[0] != '.') {
      curr_file[i] = (long)dir_content->d_name;
      ++i;
    }
  }

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
