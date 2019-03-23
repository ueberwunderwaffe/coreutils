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

#define SIZE 256

#define YELLOW_COLOR "\e[33;1m"
#define BLUE_COLOR "\e[36;1m"
#define RESET_COLOR "\e[m"

struct file_data {
  char permission[SIZE];
  int hard_links;
  char user_name[SIZE];
  char group_name[SIZE];
  int block_size;
  long long file_size;
  char date[SIZE];
};

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
int set_flags(int, char **);
int data_filling(const char *, const char **, int);
long long total(const char **, int);
int print(const char *, const char **, int);
int file_management(char *, char **, DIR *, struct dirent *);

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

int set_flags(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'l' && argv[i][2] == '\0') {
        flags.l_flag = TRUE;
      } else if (argv[i][1] == 'f' && argv[i][2] == '\0') {
        flags.a_flag = TRUE;
        flags.f_flag = TRUE;
      } else if (argv[i][1] == 'F' && argv[i][2] == '\0') {
        flags.F_flag = TRUE;
      } else if (argv[i][1] == 'a' && argv[i][2] == '\0') {
        flags.a_flag = TRUE;
      } else if (argv[i][1] == 'R' && argv[i][2] == '\0') {
        flags.R_flag = TRUE;
      } else if (argv[i][1] == 'd' && argv[i][2] == '\0') {
        flags.d_flag = TRUE;
      } else if (argv[i][1] == 't' && argv[i][2] == '\0') {
        flags.t_flag = TRUE;
      } else if (argv[i][1] == 'h' && argv[i][2] == '\0') {
        flags.h_flag = TRUE;
      }
    } else {
      printf("ls: cannot access '%s': No such file or directory\n", argv[i]);
      return (FALSE);
    }
  }

  return (TRUE);
}

int data_filling(const char *curr_dir, const char **file_name, int num_files) {
  struct file_data *files = NULL;
  files = (struct file_data *)malloc(sizeof(struct file_data) * num_files);
  if (files == NULL) {
    printf("[FAILED]: Memory allocation\n");
    free(files);
    return FALSE;
  }

  for (int i = 0; i < num_files; ++i) {
    struct stat st;
    int fd = -1;

    char *address = (char *)malloc(sizeof(char) * SIZE);
    memset(address, 0, sizeof(char));
    strcat(address, curr_dir);
    strcat(address, "/");
    strcat(address, file_name[i]);

    fd = open(address, O_RDONLY, 0);
    if (fd == -1) {
      printf("[FAILED data_filling()]: Opening file/directory\n");
      free(address);
      return (FALSE);
    }
    free(address);

    if (fstat(fd, &st)) {
      printf("[FAILED]: fstat()\n");
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

    // Date and time
    char date[SIZE];
    memset(date, 0, sizeof(date));
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

  return TRUE;
}

long long total(const char **file_name, int num_files) {
  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files * sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED]: Memory allocation\n");
    free(files);
    return FALSE;
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
    printf("total %lld\n", total((const char **)file_name, num_files));
  }

  struct file_data *files = NULL;
  files = (struct file_data *)malloc(num_files * sizeof(struct file_data));
  if (files == NULL) {
    printf("[FAILED]: Memory allocation\n");
    free(files);
    return FALSE;
  }

  for (int i = 0; i < num_files; ++i) {
    if (file_name[i] != NULL) {
      struct stat st;
      int fd = -1;

      char *address = (char *)malloc(sizeof(char) * SIZE);
      memset(address, 0, sizeof(char));
      strcat(address, curr_dir);
      strcat(address, "/");
      strcat(address, file_name[i]);

      fd = open(address, O_RDONLY, 0);
      if (fd == -1) {
        printf("[FAILED print()]: Opening file/directory\n");
        free(address);
        return (FALSE);
      }
      free(address);

      if (fstat(fd, &st)) {
        printf("[FAILED]: fstat()\n");
        close(fd);
        return (FALSE);
      }

      if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
        if (flags.l_flag) {
          printf("%s", files[i].permission);
          printf(" %4d ", files[i].hard_links);
          printf("%s ", files[i].user_name);
          printf("%s ", files[i].group_name);
          printf("%5lld ", files[i].file_size);
          printf("%s ", files[i].date);
        }

        // Check if the file is executable
        if (stat((const char *)file_name[i], &st) == 0 &&
            st.st_mode & S_IXUSR) {
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
  }
  putchar('\n');

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
    return (0);
  } else {
    file_name = (char **)malloc(num_files * sizeof(char));
    for (int i = 0; i < num_files; i++)
      file_name[i] = (char *)malloc(SIZE * sizeof(char));

    if (file_name == NULL) {
      printf("[FAILED]: Memory allocation\n");

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
    printf("[ERROR]: Couldn't open the directory");
    closedir(dir_pointer);
    free(dir_content);
    free(file_name);
    return (FALSE);
  }

  for (int i = 0; (dir_content = readdir(dir_pointer)) != NULL; ++i) {
    file_name[i] = dir_content->d_name;
  }
  free(dir_content);

  if (flags.f_flag == FALSE) {
    // Sort in alphabetical order.
    qsort(file_name, num_files, sizeof(const char **), compare);
  }

  data_filling((const char *)curr_dir, (const char **)file_name, num_files);

  int print_error =
      print((const char *)curr_dir, (const char **)file_name, num_files);
  if (print_error == FALSE) {
    printf("[FAILED]: print()\n");
    closedir(dir_pointer);
    free(file_name);
    return FALSE;
  }

  if (flags.R_flag) {
    for (int i = 0; i < num_files; ++i) {
      struct stat st;
      int fd = -1;

      char *address = (char *)malloc(sizeof(char) * SIZE);
      memset(address, 0, sizeof(char));
      strcat(address, curr_dir);
      strcat(address, "/");
      strcat(address, file_name[i]);

      fd = open(address, O_RDONLY, 0);
      if (fd == -1) {
        printf("[FAILED file_management()]: Opening file/directory\n");
        return (FALSE);
      }
      free(address);

      if (fstat(fd, &st)) {
        printf("[FAILED]: fstat()\n");
        close(fd);
        return (FALSE);
      }

      if (file_name[i][0] != '.' || flags.f_flag || flags.a_flag) {
        if (S_ISDIR(st.st_mode)) {
          strcat(curr_dir, "/");
          strcat(curr_dir, file_name[i]);

          file_name = NULL;
          dir_pointer = NULL;
          dir_content = NULL;

          int error =
              file_management(curr_dir, file_name, dir_pointer, dir_content);
          if (error == FALSE) {
            printf("[ERROR]: file_management()");
            closedir(dir_pointer);
            close(fd);
          } else {
            break;
          }
        }
      }
      close(fd);
    }
  }

  closedir(dir_pointer);

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
    closedir(dir_pointer);
    free(file_name);
    free(curr_dir);
    free(dir_content);
    return (-1);
  }

  curr_dir = getenv("PWD");
  if (curr_dir == NULL) {
    printf("[ERROR]: Couldn't get the directory\n");
    return (-1);
  }

  int file_management_error =
      file_management(curr_dir, file_name, dir_pointer, dir_content);
  if (file_management_error == FALSE) {
    printf("[FAILED]: file_management()\n");
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
