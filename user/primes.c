#include "kernel/types.h"
#include "user/user.h"

void sieve(int source_pipe[2]) {
    close(source_pipe[1]);

    int prime;
    int check_read = read(source_pipe[0], &prime, 4);
    if (check_read == 0) {
        exit(0);
    }
    else if (check_read != 4) {
        fprintf(2, "Error: error reading into sieve\n");
        exit(1);
    }

    printf("prime %d\n", prime);

    int sieve_pipe[2];
    pipe(sieve_pipe);  

    int sieve_pid = fork();
    if (sieve_pid == 0) {
        sieve(sieve_pipe);
    }
    else if (sieve_pid > 0) {
        int num;
        while (read(source_pipe[0], &num, 4) == 4) {
            if (num % prime != 0) {
                write(sieve_pipe[1], &num, 4);
            }
        }
        close(sieve_pipe[1]);
    }
    else
    {
        fprintf(2, "Error: error forking sieve\n");
        exit(1);
    }

}

int main() {
    int feed_pipe[2];

    pipe(feed_pipe);

    int pid = fork();
    if (pid == 0) {
        sieve(feed_pipe);
    }
    else if (pid > 0) {
        for (int i = 2; i <= 35; i++) {
            if (write(feed_pipe[1], &i, sizeof(i)) != 4) {
                fprintf(2, "Error: error feeding numbers\n");
                exit(1);
            }
        }
    }
    else {
        fprintf(2, "Error: error forking feed process\n");
        exit(1);
    }

    close(feed_pipe[1]);
    wait(0);
    exit(0);
}