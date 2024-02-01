#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc != 2) // incorrect number of arguments
    {
        fprintf(2, "Usage: sleep ticks\n");
        exit(1);
    }

    int n_ticks = atoi(argv[1]); // convert tick argument to int
    if (n_ticks <= 0) // avoid non-positive number of ticks
    {
        fprintf(2, "Error: Invalid number of ticks\n");
        exit(1);
    }

    sleep(n_ticks); // call sleep from user.h

    exit(0); // sleep success
}