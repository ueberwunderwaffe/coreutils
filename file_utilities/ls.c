#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
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

int flags(int argc, char **argv, int *l_flag, int *f_flag, int *F_flag,
          int *a_flag, int *R_flag, int *d_flag, int *t_flag, int *h_flag) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'l' && argv[i][2] == '\0') {
        *l_flag = TRUE;
      } else if (argv[i][1] == 'f' && argv[i][2] == '\0') {
        *a_flag = TRUE;
        *f_flag = TRUE;
      } else if (argv[i][1] == 'F' && argv[i][2] == '\0') {
        *F_flag = TRUE;
      } else if (argv[i][1] == 'a' && argv[i][2] == '\0') {
        *a_flag = TRUE;
      } else if (argv[i][1] == 'R' && argv[i][2] == '\0') {
        *R_flag = TRUE;
      } else if (argv[i][1] == 'd' && argv[i][2] == '\0') {
        *d_flag = TRUE;
      } else if (argv[i][1] == 't' && argv[i][2] == '\0') {
        *t_flag = TRUE;
      } else if (argv[i][1] == 'h' && argv[i][2] == '\0') {
        *h_flag = TRUE;
      }
    } else {
      printf("ls: cannot access '%s': No such file or directory\n", argv[i]);
      return (FALSE);
    }
  }

  return (TRUE);
}

int print(const char **curr_file, int num_files, int *l_flag, int *F_flag,
          int *a_flag, int *R_flag, int *d_flag, int *t_flag, int *h_flag) {
  for (int i = 0; i < num_files; ++i) {
    int fd = -1;
    struct stat st;

    fd = open(curr_file[i], O_RDONLY, 0);
    if (fd == -1) {
      printf("[FAILED]: Opening file/directory\n");
      return (FALSE);
    }

    if (fstat(fd, &st)) {
      printf("[FAILED]: fstat()\n");
      close(fd);
      return (FALSE);
    }

    if (curr_file[i][0] != '.' || *a_flag) {
      if (*l_flag) {
        mode_t permission;

        if (S_ISDIR(st.st_mode))
          putchar('d');
        else
          putchar('-');

        // Owner
        permission = st.st_mode & S_IRWXU;

        if (permission & S_IRUSR)
          putchar('r');
        else
          putchar('-');

        if (permission & S_IWUSR)
          putchar('w');
        else
          putchar('-');

        if (permission & S_IXUSR)
          putchar('x');
        else
          putchar('-');

        // Group
        permission = st.st_mode & S_IRWXG;

        if (permission & S_IRGRP)
          putchar('r');
        else
          putchar('-');

        if (permission & S_IWGRP)
          putchar('w');
        else
          putchar('-');

        if (permission & S_IXGRP)
          putchar('x');
        else
          putchar('-');

        // Others
        permission = st.st_mode & S_IRWXO;

        if (permission & S_IROTH)
          putchar('r');
        else
          putchar('-');

        if (permission & S_IWOTH)
          putchar('w');
        else
          putchar('-');

        if (permission & S_IXOTH)
          putchar('x');
        else
          putchar('-');

        // Number of hard links
        printf(" %d ", (int)st.st_nlink);

        // User name
        struct passwd *user_name = getpwuid(st.st_uid);
        printf("%s ", user_name->pw_name);

        // Group name
        struct group *group_name = getgrgid(st.st_gid);
        printf("%s ", group_name->gr_name);

        // File size
        printf("%5lld ", (long long)st.st_size);

        // Date and time
        char time[100];
        memset(time, 0, sizeof(time));
        strcpy(time, ctime(&st.st_ctime));
        for (int i = 0; time[i] != '\0'; i++) {
          if (time[i] == '\n') {
            time[i] = '\0';
            break;
          }
        }
        printf("%s ", time);
      }

      // Check if the file is executable
      if (stat((const char *)curr_file[i], &st) == 0 && st.st_mode & S_IXUSR) {
        if (S_ISDIR(st.st_mode)) {
          if (*F_flag)
            printf(BLUE_COLOR "%s" RESET_COLOR "/  ", curr_file[i]);
          else
            printf(BLUE_COLOR "%s  " RESET_COLOR, curr_file[i]);
        } else {
          if (*F_flag)
            printf(YELLOW_COLOR "%s" RESET_COLOR "*  ", curr_file[i]);
          else
            printf(YELLOW_COLOR "%s  " RESET_COLOR, curr_file[i]);
        }
        close(fd);
      } else {
        printf("%s  ", curr_file[i]);
      }

      if (*l_flag && i != num_files - 1)
        putchar('\n');
    }
  }
  putchar('\n');

  return (TRUE);
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
      free(dir_content);
      closedir(dir_pointer);

      return (-1);
    }
  }

  dir_pointer = opendir((const char *)curr_dir);
  if (dir_pointer == NULL) {
    printf("[ERROR]: Couldn't open the directory");
    closedir(dir_pointer);
    free(dir_content);
    free(curr_file);
    return (-1);
  }

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL; ++i) {
    curr_file[i] = dir_content->d_name;
  }
  free(dir_content);

                       ///////////
                      // FLAGS //
                     ///////////
  
  int l_flag = FALSE; // long format, displaying Unix file types, permissions,
                      // number of hard links, owner, group, size,
                      // last-modified date and filename.

  int f_flag = FALSE; // do not sort. Useful for directories containing large
                      // numbers of files.

  int F_flag = FALSE; // appends a character revealing the nature of a file, for
                      // example, * for an executable, or / for a directory.
                      // Regular files have no suffix.

  int a_flag = FALSE; // lists all files in the given directory,
                      // including those whose names start with "."
                      // (which are hidden files in Unix).

  int R_flag = FALSE; // recursively lists subdirectories.

  int d_flag = FALSE; // shows information about a symbolic link or directory,
                      // rather than about the link's target or listing the
                      // contents of a directory.

  int t_flag = FALSE; // sort the list of files by modification time.

  int h_flag = FALSE; // print sizes in human readable format. [This option is
                      // not part of the POSIX standard]

  int flag_error = flags(argc, argv, &l_flag, &f_flag, &F_flag, &a_flag,
                         &R_flag, &d_flag, &t_flag, &h_flag);
  if (flag_error == FALSE) {
    closedir(dir_pointer);
    free(curr_file);
    return (-1);
  }

  if (f_flag == FALSE) {
    // Sort in alphabetical order.
    qsort(curr_file, num_files, sizeof(const char **), compare);
  }

  int print_error = print((const char **)curr_file, num_files, &l_flag, &F_flag,
                          &a_flag, &R_flag, &d_flag, &t_flag, &h_flag);
  if (print_error == FALSE) {
    closedir(dir_pointer);
    free(curr_file);
    return (-1);
  }

  closedir(dir_pointer);
  free(curr_file);

  return (0);
}
