#include "kernel/types.h"
#include "user/user.h"

void
seive(int pipe_in[])
{
  int n, buf, pipe_out[2];
  close(pipe_in[1]);

  pipe(pipe_out);

  if (read(pipe_in[0], &n, sizeof(int)) > 0) {
    printf("prime %d\n", n);

    if (fork() == 0) {
      seive(pipe_out);
      exit(0);
    } else {
      close(pipe_out[0]);
      while (read(pipe_in[0], &buf, sizeof(int)) > 0) {
        if (buf % n != 0) {
          write(pipe_out[1], &buf, sizeof(int));
        }
      }
      close(pipe_out[1]);
      wait(0);
    }
  }
  exit(0);
}

int
main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(2, "Usage: primes\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  if (fork() == 0) {
    seive(p);
    exit(0);
  } else {
    close(p[0]);
    for (int i = 2; i <= 35; ++i) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
    close(p[0]);
  }

  exit(0);
}