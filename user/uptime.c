#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc != 1)
    {
        fprintf(1, "Usage: uptime\n");
        exit(0);
    };

    int ticks = uptime();
    fprintf(1, "%d\n", ticks);

    exit(0);
};