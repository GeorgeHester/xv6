#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    fprintf(1, "Unix V6 was released in the year %d\n", getyear());
    exit(0);
};