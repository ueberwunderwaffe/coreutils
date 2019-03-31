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

#define ERROR -1

#define SIZE 256

#define YELLOW_COLOR "\e[33;1m"
#define BLUE_COLOR "\e[36;1m"
#define RESET_COLOR "\e[m"

struct file_data {
  char name[SIZE];
  char permission[SIZE];
  int hard_links;
  char user_name[SIZE];
  char group_name[SIZE];
  int block_size;
  long long file_size;
  char date[SIZE];
  time_t time;
};

int num_files_compare_time = -1;

struct command_flags {
  int l_flag; // long format, displaying Unix file types, permissions,
              // number of hard links, owner, group, size,
              // last-modified date and filename.

  int f_flag; // do not sort. Useful for directories containing large
              // numbers of files.

  int F_flag; // appends a character revealing the nature of a file,
              // for example, * for an executable, or / for a
              // directory. Regular files have no suffix.

  int a_flag; // lists all files in the given directory,
              // including those whose names start with "."
              // (which are hidden files in Unix).

  int R_flag; // recursively lists subdirectories.

  int d_flag; // shows information about a symbolic link or directory,
              // rather than about the link's target or listing the
              // contents of a directory.

  int t_flag; // sort the list of files by modification time.

  int h_flag; // print sizes in human readable format. [This option is
              // not part of the POSIX standard]
} flags;

int compare(const void *, const void *);
int compare_time(const void *, const void *);
int column_max_width(const char **, const struct file_data *, int, char);
int set_flags(int, char **);
int set_time_and_name(char *, char **, int);
int data_filling(const char *, const char **, int);
long long total(const char **, int);
int print(const char *, const char **, int);
int file_management(char *, char **, DIR *, struct dirent *);

int compare(const void *a, const void *b) {
  char *first = NULL;
  first = (char *)malloc(strlen(*(char **)a) * sizeof(char));
  if (first == NULL) {
    printf("[FAILED compare()]: Memory allocation\n");
    free(first);
    return FALSE;
  }

  char *second = NULL;
  second = (char *)malloc(strlen(*(char **)b) * sizeof(char));
  if (second == NULL) {
    printf("[FAILED compare()]: Memory allocation\n");
    free(second);
    return FALSE;
  }

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

int compare_time(const void *a, const void *b) {
  if (num_files_compare_time == -1) {
    printf("[FAILED compare_time()]: Variable [num_files_compare_time] isn't "
           "set\n");
    return (FALSE);
  }

  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files_compare_time *
                                     sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED compare_time()]: Memory allocation\n");
    free(files);
    return (FALSE);
  }

  int a_index = -1;
  int b_index = -1;

  for (int i = 0; i < num_files_compare_time; ++i) {
    if (strcmp(files[i].name, *(char **)a) == 0)
      a_index = i;
    else if (strcmp(files[i].name, *(char **)b) == 0)
      b_index = i;
  }

  free(files);

  return difftime(files[b_index].time, files[a_index].time);
}

int column_max_width(const char **file_name, const struct file_data *files,
                     int num_files, char mode) {
  int counter = 0;
  int size = 0;
  int value = 0;

  for (int i = 0; i < num_files; i++) {
    if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
      counter = 0;
      value = (mode == 'h') ? files[i].hard_links : files[i].file_size;
      while (value > 0) {
        value /= 10;
        counter++;
      }

      if (counter > size)
        size = counter;
    }
  }

  return size;
}

int set_flags(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'l' && argv[i][2] == '\0') {
        flags.l_flag = TRUE;
      } else if (argv[i][1] == 'f' && argv[i][2] == '\0') {
        flags.a_flag = TRUE;
        flags.f_flag = TRUE;
      } else if ((argv[i][1] == 'F' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--classify") == 0) {
        flags.F_flag = TRUE;
      } else if ((argv[i][1] == 'a' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--all") == 0) {
        flags.a_flag = TRUE;
      } else if ((argv[i][1] == 'R' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--recursive") == 0) {
        flags.R_flag = TRUE;
      } else if ((argv[i][1] == 'd' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--directory") == 0) {
        flags.d_flag = TRUE;
      } else if (argv[i][1] == 't' && argv[i][2] == '\0') {
        flags.t_flag = TRUE;
      } else if ((argv[i][1] == 'h' && argv[i][2] == '\0') ||
                 strcmp(argv[i], "--human-readable") == 0) {
        flags.h_flag = TRUE;
      }
    } else {
      printf("ls: cannot access '%s': No such file or directory\n", argv[i]);
      return (FALSE);
    }
  }

  return (TRUE);
}

int set_time_and_name(char *curr_dir, char **file_name, int num_files) {
  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files_compare_time *
                                     sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED set_time_and_name()]: Memory allocation\n");
    free(files);
    return (FALSE);
  }

  char *path = NULL;
  path = (char *)malloc(sizeof(char) * SIZE);
  if (path == NULL) {
    printf("[FAILED set_time_and_name()]: Memory allocation\n");
    free(files);
    free(path);
    return (FALSE);
  }

  for (int i = 0; i < num_files; ++i) {
    struct stat st;
    int fd = -1;

    memset(path, 0, sizeof(char) * SIZE);
    strcat(path, curr_dir);
    strcat(path, "/");
    strcat(path, file_name[i]);

    fd = open(path, O_RDONLY, 0);
    if (fd == -1) {
      printf("[FAILED set_time_and_name()]: Opening file/directory\n");
      free(files);
      free(path);
      return (FALSE);
    }

    if (fstat(fd, &st)) {
      printf("[FAILED set_time_and_name()]: fstat()\n");
      free(files);
      free(path);
      close(fd);
      return (FALSE);
    }

    // File name
    strcpy(files[i].name, file_name[i]);

    // Time
    files[i].time = st.st_mtime;

    close(fd);
  }

  free(files);
  free(path);

  return (TRUE);
}

int data_filling(const char *curr_dir, const char **file_name, int num_files) {
  struct file_data *files = NULL;
  files = (struct file_data *)malloc(sizeof(struct file_data) * num_files);
  if (files == NULL) {
    printf("[FAILED data_filling()]: Memory allocation\n");
    free(files);
    return (FALSE);
  }

  char *path = NULL;
  path = (char *)malloc(sizeof(char) * SIZE);
  if (path == NULL) {
    printf("[FAILED data_filling()]: Memory allocation\n");
    free(files);
    free(path);
    return (FALSE);
  }

  for (int i = 0; i < num_files; ++i) {
    struct stat st;
    int fd = -1;

    memset(path, 0, sizeof(char) * SIZE);
    strcat(path, curr_dir);
    strcat(path, "/");
    strcat(path, file_name[i]);

    fd = open(path, O_RDONLY, 0);
    if (fd == -1) {
      printf("[FAILED data_filling()]: Opening file/directory\n");
      free(files);
      free(path);
      return (FALSE);
    }

    if (fstat(fd, &st)) {
      printf("[FAILED data_filling()]: fstat()\n");
      free(files);
      free(path);
      close(fd);
      return (FALSE);
    }

    int perm_index = 0;
    mode_t permission;

    if (S_ISDIR(st.st_mode))
      files[i].permission[perm_index++] = 'd';
    else
      files[i].permission[perm_index++] = '-';

    // Owner
    permission = st.st_mode & S_IRWXU;

    if (permission & S_IRUSR)
      files[i].permission[perm_index++] = 'r';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IWUSR)
      files[i].permission[perm_index++] = 'w';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IXUSR)
      files[i].permission[perm_index++] = 'x';
    else
      files[i].permission[perm_index++] = '-';

    // Group
    permission = st.st_mode & S_IRWXG;

    if (permission & S_IRGRP)
      files[i].permission[perm_index++] = 'r';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IWGRP)
      files[i].permission[perm_index++] = 'w';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IXGRP)
      files[i].permission[perm_index++] = 'x';
    else
      files[i].permission[perm_index++] = '-';

    // Others
    permission = st.st_mode & S_IRWXO;

    if (permission & S_IROTH)
      files[i].permission[perm_index++] = 'r';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IWOTH)
      files[i].permission[perm_index++] = 'w';
    else
      files[i].permission[perm_index++] = '-';

    if (permission & S_IXOTH)
      files[i].permission[perm_index++] = 'x';
    else
      files[i].permission[perm_index++] = '-';

    // Number of hard links
    files[i].hard_links = (int)st.st_nlink;

    // User name
    struct passwd *user_name = getpwuid(st.st_uid);
    strcpy(files[i].user_name, user_name->pw_name);

    // Group name
    struct group *group_name = getgrgid(st.st_gid);
    strcpy(files[i].group_name, group_name->gr_name);

    // Block size
    files[i].block_size = (int)st.st_blksize;

    // File size
    files[i].file_size = (long long)st.st_size;

    // Date
    char date[SIZE];
    memset(date, 0, sizeof(char) * SIZE);
    strcpy(date, ctime(&st.st_ctime));
    for (int i = 0; date[i] != '\0'; i++) {
      if (date[i] == '\n') {
        date[i] = '\0';
        break;
      }
    }
    strcpy(files[i].date, date);

    close(fd);
  }

  free(files);
  free(path);

  return (TRUE);
}

long long total(const char **file_name, int num_files) {
  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files * sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED total()]: Memory allocation\n");
    free(files);
    return (FALSE);
  }

  long long result = 0;
  int sys_block_size = 0;
  int file_size = 0;
  int file_blocks = 0;

  for (int i = 0; i < num_files; i++) {
    if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
      sys_block_size = files[i].block_size;
      file_size = files[i].file_size;
      file_blocks = 0;

      while (file_size > 0) {
        file_size -= sys_block_size;
        file_blocks++;
      }

      result += file_blocks;
    }
  }

  free(files);

  return result * ((sys_block_size / 512) / 2); // ???
}

int print(const char *curr_dir, const char **file_name, int num_files) {
  if (flags.l_flag) {
    if (flags.h_flag)
      printf("total %lldK\n", total((const char **)file_name, num_files));
    else
      printf("total %lld\n", total((const char **)file_name, num_files));
  }

  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files * sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED print()]: Memory allocation\n");
    free(files);
    return (FALSE);
  }

  char *path = NULL;
  path = (char *)malloc(sizeof(char) * SIZE);
  if (path == NULL) {
    printf("[FAILED print()]: Memory allocation\n");
    free(files);
    free(path);
    return (FALSE);
  }

  for (int i = 0; i < num_files; ++i) {
    struct stat st;
    int fd = -1;

    memset(path, 0, sizeof(char) * SIZE);
    strcat(path, curr_dir);
    strcat(path, "/");
    strcat(path, file_name[i]);

    fd = open(path, O_RDONLY, 0);
    if (fd == -1) {
      printf("[FAILED print()]: Opening file/directory\n");
      free(files);
      free(path);
      return (FALSE);
    }

    if (fstat(fd, &st)) {
      printf("[FAILED printf()]: fstat()\n");
      free(files);
      free(path);
      close(fd);
      return (FALSE);
    }

    if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
      if (flags.l_flag) {
        printf("%s", files[i].permission);
        printf(" %*d ", column_max_width(file_name, files, num_files, 'h'),
               files[i].hard_links);
        printf("%s ", files[i].user_name);
        printf("%s ", files[i].group_name);

        if (flags.h_flag) {
          long double f_size = files[i].file_size;
          int max_width = 4;
          if (f_size > 1099511627776) {
            f_size /= 1099511627776;
            printf("%*.1lfT ", max_width, (double)f_size);
          } else {
            if (f_size > 1073741824) {
              f_size /= 1073741824;
              printf("%*.1lfG ", max_width, (double)f_size);
            } else {
              if (f_size > 1048576) {
                f_size /= 1048576;
                printf("%*.1lfM ", max_width, (double)f_size);
              } else {
                if (f_size > 1024) {
                  f_size /= 1024;
                  printf("%*.1lfK ", max_width, (double)f_size);
                } else {
                  printf("%*lld ", max_width + 1, (long long)f_size);
                }
              }
            }
          }
        } else {
          printf("%*lld ", column_max_width(file_name, files, num_files, 'f'),
                 files[i].file_size);
        }

        printf("%s ", files[i].date);
      }

      // Check if the file is executable
      if (stat((const char *)path, &st) == 0 && st.st_mode & S_IXUSR) {
        if (S_ISDIR(st.st_mode)) {
          if (flags.F_flag)
            printf(BLUE_COLOR "%s" RESET_COLOR "/  ", file_name[i]);
          else
            printf(BLUE_COLOR "%s  " RESET_COLOR, file_name[i]);
        } else {
          if (flags.F_flag)
            printf(YELLOW_COLOR "%s" RESET_COLOR "*  ", file_name[i]);
          else
            printf(YELLOW_COLOR "%s  " RESET_COLOR, file_name[i]);
        }
        close(fd);
      } else {
        printf("%s  ", file_name[i]);
      }

      if (flags.l_flag && i != num_files - 1)
        putchar('\n');
    }
  }
  putchar('\n');

  free(path);
  free(files);

  return (TRUE);
}

int file_management(char *curr_dir, char **file_name, DIR *dir_pointer,
                    struct dirent *dir_content) {
  int num_files = 0;
  dir_pointer = opendir((const char *)curr_dir);
  while ((dir_content = readdir(dir_pointer)) != NULL)
    num_files++;
  closedir(dir_pointer);

  dir_pointer = NULL;
  dir_content = NULL;

  if (num_files == 0) {
    return (TRUE);
  } else {
    file_name = (char **)malloc(num_files * sizeof(char *));
    if (file_name == NULL) {
      printf("[FAILED file_management()]: Memory allocation\n");

      for (int i = 0; i < num_files; i++)
        free(file_name[i]);
      free(file_name);
      free(dir_content);
      closedir(dir_pointer);

      return (FALSE);
    }

    for (int i = 0; i < num_files; i++) {
      file_name[i] = NULL;
      file_name[i] = (char *)malloc(SIZE * sizeof(char));

      if (file_name[i] == NULL) {
        printf("[FAILED file_management()]: Memory allocation\n");

        for (int j = 0; j < i; j++)
          free(file_name[j]);
        free(file_name);
        free(dir_content);
        closedir(dir_pointer);

        return (FALSE);
      }
    }

    if (file_name == NULL) {
      printf("[FAILED file_management()]: Memory allocation\n");

      for (int i = 0; i < num_files; i++)
        free(file_name[i]);
      free(file_name);
      free(dir_content);
      closedir(dir_pointer);

      return (FALSE);
    }
  }

  dir_pointer = opendir((const char *)curr_dir);
  if (dir_pointer == NULL) {
    printf("[ERROR file_management()]: Couldn't open the directory");
    closedir(dir_pointer);
    free(dir_content);
    for (int i = 0; i < num_files; i++)
      free(file_name[i]);
    free(file_name);
    return (FALSE);
  }

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL; ++i) {
    strcpy(file_name[i], dir_content->d_name);
  }
  free(dir_content);

  if (flags.f_flag == FALSE) {
    if (flags.t_flag == TRUE) {
      // Sort by modification time.
      num_files_compare_time = num_files;
      set_time_and_name(curr_dir, file_name, num_files);
      qsort(file_name, num_files_compare_time, sizeof(const char **),
            compare_time);
    } else {
      // Sort in alphabetical order.
      qsort(file_name, num_files, sizeof(const char **), compare);
    }
  }

  int data_filling_error =
      data_filling((const char *)curr_dir, (const char **)file_name, num_files);
  if (data_filling_error == FALSE) {
    printf("[FAILED file_management()]: data_filling()\n");
    closedir(dir_pointer);
    for (int i = 0; i < num_files; i++)
      free(file_name[i]);
    free(file_name);
    return (FALSE);
  }

  int print_error =
      print((const char *)curr_dir, (const char **)file_name, num_files);
  if (print_error == FALSE) {
    printf("[FAILED file_management()]: print()\n");
    closedir(dir_pointer);
    for (int i = 0; i < num_files; i++)
      free(file_name[i]);
    free(file_name);
    return (FALSE);
  }

  if (flags.R_flag) {
    char *path = NULL;
    path = (char *)malloc(sizeof(char) * SIZE);
    if (path == NULL) {
      printf("[FAILED file_management()]: Memory allocation\n");
      closedir(dir_pointer);
      for (int i = 0; i < num_files; i++)
        free(file_name[i]);
      free(file_name);
      free(path);

      return (FALSE);
    }

    char *new_curr_dir = NULL;
    new_curr_dir = (char *)malloc(sizeof(char) * SIZE);
    if (new_curr_dir == NULL) {
      printf("[FAILED file_management()]: Memory allocation\n");
      closedir(dir_pointer);
      for (int i = 0; i < num_files; i++)
        free(file_name[i]);
      free(file_name);
      free(path);

      return (FALSE);
    }

    for (int i = 0; i < num_files; ++i) {
      struct stat st;
      int fd = -1;

      memset(path, 0, sizeof(char) * SIZE);
      strcat(path, curr_dir);
      strcat(path, "/");
      strcat(path, file_name[i]);

      fd = open(path, O_RDONLY, 0);
      if (fd == -1) {
        printf("[FAILED file_management()]: Opening file/directory\n");
        closedir(dir_pointer);
        for (int i = 0; i < num_files; i++)
          free(file_name[i]);
        free(file_name);
        free(path);
        return (FALSE);
      }

      if (fstat(fd, &st)) {
        printf("[FAILED file_management()]: fstat()\n");
        closedir(dir_pointer);
        for (int i = 0; i < num_files; i++)
          free(file_name[i]);
        free(file_name);
        free(path);
        close(fd);
        return (FALSE);
      }

      if (S_ISDIR(st.st_mode) && file_name[i][0] != '.')
        printf("\n.%s:\n", path);

      if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
        if (S_ISDIR(st.st_mode)) {
          memset(new_curr_dir, 0, sizeof(char) * SIZE);
          strcat(new_curr_dir, curr_dir);
          strcat(new_curr_dir, "/");
          strcat(new_curr_dir, file_name[i]);

          char **new_file_name = NULL;
          dir_pointer = NULL;
          dir_content = NULL;

          int error = file_management(new_curr_dir, new_file_name, dir_pointer,
                                      dir_content);
          if (error == FALSE) {
            printf("[ERROR file_management()]: file_management()");
            closedir(dir_pointer);
            close(fd);

            for (int i = 0; i < num_files; i++)
              free(file_name[i]);
            free(file_name);
            free(new_file_name);

            return (FALSE);
          } else {
            free(new_file_name);
          }
        }
      }

      close(fd);
    }

    free(path);
    free(new_curr_dir);
  }

  closedir(dir_pointer);

  for (int i = 0; i < num_files; i++)
    free(file_name[i]);
  free(file_name);

  return (TRUE);
}

int main(int argc, char **argv) {
  char *curr_dir = NULL;
  char **file_name = NULL;
  DIR *dir_pointer = NULL;
  struct dirent *dir_content = NULL;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  flags.l_flag = FALSE;
  flags.f_flag = FALSE;
  flags.F_flag = FALSE;
  flags.a_flag = FALSE;
  flags.R_flag = FALSE;
  flags.d_flag = FALSE;
  flags.t_flag = FALSE;
  flags.h_flag = FALSE;

  int flag_error = set_flags(argc, argv);
  if (flag_error == FALSE) {
    printf("[ERROR main()]: Couldn't set the flags\n");
    closedir(dir_pointer);
    free(file_name);
    free(curr_dir);
    free(dir_content);
    return (-1);
  }

  curr_dir = getenv("PWD");
  if (curr_dir == NULL) {
    printf("[ERROR main()]: Couldn't get the directory\n");
    return (-1);
  }

  int file_management_error =
      file_management(curr_dir, file_name, dir_pointer, dir_content);
  if (file_management_error == FALSE) {
    printf("[FAILED main()]: file_management()\n");
    closedir(dir_pointer);
    free(file_name);
    free(curr_dir);
    free(dir_content);
    return (-1);
  }

  closedir(dir_pointer);
  free(file_name);

  return (0);
}
