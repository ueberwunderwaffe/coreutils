#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int number_analyzer(char *);
int print(int, char **);

int main(int argc, char **argv) {
  if (print(argc, argv) == ERROR)
    return (ERROR);

  return (0);
}

int number_analyzer(char *number) {
  int length = strlen(number);
  int dec = 1;
  int result = 0;

  for (int i = length - 1; i >= 0; --i) {
    if (number[i] > 47 && number[i] < 58) {
      result += (number[i] - '0') * dec;
      dec *= 10;
    } else {
      if (number[0] == '+')
        continue;
      else
        return (ERROR);
    }
  }

  return (result);
}

int print(int argc, char **argv) {
  int ignore_num = 0;
  int extra_flag = TRUE;
  int extra_index = 1;

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        extra_flag = FALSE;
        if (strcmp(argv[i], "--ignore") == 0) {
          if (i != argc - 1) {
            i++;
            ignore_num = number_analyzer(argv[i]);
            if (ignore_num == ERROR) {
              printf("nproc: invalid number: ’%s’\n", argv[i]);
              return (ERROR);
            }
          } else {
            printf("nproc: option '--ignore' requires an argument\n");
            printf("Try 'nproc --help' for more information.\n");
            return (ERROR);
          }
        } else {
          printf("nproc: invalid option -- '%c'\n", argv[i][1]);
          printf("Try 'nproc --help' for more information.\n");
          return (ERROR);
        }
      } else {
        extra_flag = TRUE;
        extra_index = i;
      }
    }

    if (extra_flag) {
      printf("nproc: extra operand ’%s’\n", argv[extra_index]);
      printf("Try 'nproc --help' for more information.\n");
      return (ERROR);
    }
  }

  long result = sysconf(_SC_NPROCESSORS_ONLN) - ignore_num;
  if (result < 2)
    printf("%d\n", 1);
  else
    printf("%ld\n", result);

  return (TRUE);
}
