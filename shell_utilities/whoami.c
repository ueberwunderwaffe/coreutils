#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ERROR -1

int main(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        printf("whoami: invalid option -- '%c'\n", argv[i][1]);
        printf("Try 'whoami --help' for more information.\n");
        return (ERROR);
      }
    }

    printf("whoami: extra operand ’%s’\n", argv[1]);
    printf("Try 'whoami --help' for more information.\n");
    return (ERROR);
  } else {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);

    if (pw) {
      printf("%s\n", pw->pw_name);
    } else {
      printf("whoami: cannot find username for UID %u\n", uid);
      return (ERROR);
    }
  }

  return (0);
}
