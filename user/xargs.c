#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

void
copy_argv(char **ori_argv, int ori_argc, char *new_argv, char **argv)
{
  int k = 0;
  for (int i = 0; i < ori_argc; i++) {
    argv[k] = malloc(strlen(ori_argv[i]) + 1);
    memcpy(argv[k++], ori_argv[i], strlen(ori_argv[i]) + 1);
  }
  argv[k] = malloc(strlen(new_argv) + 1);
  memcpy(argv[k++], new_argv, strlen(new_argv) + 1);
}

int 
main(int argc, char *argv[])
{
  if (argc <= 1) {
    fprintf(2, "Usage: xargx command [arg ...]\n");
    exit(1);
  }

  char buf[512];  
  int i = 0;
  char ch;
  int ignore = 0;
  while (read(0, &ch, 1) > 0) { \
  // read a character at a time until a newline ('\n') appears.
    if (ch == '\n') {
      if (ignore) {
        i = 0;
        ignore = 0;
        continue;
      }
      buf[i] = 0;
      i = 0;

      if (fork() == 0) {
        // child
        int cmd_argc = argc;
        
        char *cmd_argv[MAXARG];

        copy_argv(argv + 1, argc - 1, buf, cmd_argv);
        cmd_argv[cmd_argc] = 0;
        
        exec(cmd_argv[0], cmd_argv);

        exit(0);
      } else {
        // parent
        wait(0);
      }
      
    } else {
      if (!ignore && i >= 511) {
        printf("xargs: too long arguments...\n");
        ignore = 1;
      }

      if (!ignore) {
        buf[i++] = ch;
      }
    }
  }

  exit(0);
}