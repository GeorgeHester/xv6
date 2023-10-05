#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    int ping_pipe[2];
    char byte[1];

    pipe(ping_pipe);

    int pid = fork();

    if (pid == 0)
    {
        // Read the byte from the pipe
        read(ping_pipe[0], &byte, 1);
        // Write out the byte data to the standard output file descriptor
        fprintf(1, "Data: %s\n", byte);
        // Write new byte data to the pipe
        write(ping_pipe[1], "b", 1);
        exit(0);
    }
    else
    {
        write(ping_pipe[1], "a", 1);
        wait((int*)0);
        read(ping_pipe[0], &byte, 1);
        fprintf(1, "Data: %s\n", byte);
    };

    exit(0);
};