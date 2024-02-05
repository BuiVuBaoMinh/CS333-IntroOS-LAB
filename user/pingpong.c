#include "kernel/types.h"
#include "user/user.h"

int
main()
{
    int p[2];
    char* buf = "b";

    pipe(p);

    int pid = fork();
    if (pid == 0){ 
    // child process
        if (read(p[0], &buf, 1) != 1) {
            fprintf(2, "Error: child cannot read from pipe\n");
            exit(1);
        }

        int c_pid = getpid();
        printf("%d: received ping\n", c_pid);

        write(p[1], &buf, 1);
        close(p[1]);

        exit(0);

    }
    else if (pid > 0){
    // parent process
        // send byte to child via pipe
        write(p[1], &buf, 1);
        close(p[1]);

        wait(0); // wait for child to receive and reply

        if (read(p[0], &buf, 1) != 1) {
            fprintf(2, "Error: parent cannot read from pipe\n");
            exit(1);
        }

        int p_pid = getpid();
        printf("%d: received pong\n", p_pid);
        exit(0);

    }
    else {
        fprintf(2, "Error: fork error\n");
        exit(1);
    }

}

