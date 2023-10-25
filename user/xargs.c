#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char** argv)
{
    if (argc < 2)
    {
        // Print out stdin pipe
    };

    if (argc >= 2)
    {
        char* output_argv[MAXARG];
        int argi = 1;

        while (argi < argc)
        {
            if (argi > MAXARG)
            {
                printf("Error: Too many arguments");
                exit(0);
            };

            output_argv[argi - 1] = malloc(sizeof(argv[argi]));
            strcpy(output_argv[argi - 1], argv[argi]);

            argi++;
        };

        char character;
        int character_index = 0;

        while (read(0, &character, 1))
        {
            if (character_index == 0)
            {
                if (argi > MAXARG)
                {
                    printf("Error: Too many arguments");
                    exit(0);
                };

                output_argv[argi - 1] = malloc(128);
            };

            if (character == '\n' || character_index >= 128)
            {
                if (fork() == 0)
                {
                    exec(output_argv[0], output_argv);
                }
                else
                {
                    character_index = 0;
                    free(output_argv[argi - 1]);
                    wait(0);
                };

                continue;
            };

            output_argv[argi - 1][character_index] = character;
            character_index++;
        };
    };

    exit(0);
};