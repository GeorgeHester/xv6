#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char** argv)
{
    // fprintf(1, "Argument Count: %d\n", argc);

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

        /*
                for (argi; argi < argc; argi++)
                {
                    if (argi > MAXARG - 1) break;
                };

                output_argv[argi - 1] = malloc(128);

                char character;
                int character_index = 0;

                while (read(0, &character, 1))
                {
                    if (character == '\n')
                    {

                        continue;
                    };

                    output_argv[argi - 1][character_index] = character;
                };*/
    };

    // exec(argv[1]);

    /*
        int stdin_pipe[2];
        pipe(stdin_pipe);

        // Max input is 100 chars from testing
        char buffer;
        while (read(0, &buffer, 1))
        {
            if (buffer = '\n')
            {
            };

            // printf("%c", buffer);

            write(stdin_pipe[1], buffer, 1);
        };

        for (int argi = 0; argi < argc; argi++)
        {
            printf("Argument %d: %s\n", argi, argv[argi]);
        };*/

    /*char buffer[100];
    int buffer_bytes = read(0, buffer, 100);

    fprintf(1, "STDIN: %s\n", buffer);
    fprintf(1, "BYTES: %d\n", buffer_bytes);*/

    exit(0);
    /*

    // fprintf(1, "Input Read: %s\n", pipe_input);
    // fprintf(1, "Bytes Read: %d\n", pipe_bytes);

    // int finished = 0;
    int character = 0;
    int pipe_pipe[2];
    pipe(pipe_pipe);
    int buffer_length = 0;

    while (1)
    {
        if (character >= pipe_bytes) break;

        if (pipe_input[character] == ' ')
        {
            char buffer[buffer_length];

            close(pipe_pipe[1]);
            read(pipe_pipe[0], buffer, buffer_length);
            close(pipe_pipe[0]);

            buffer_length = 0;
            pipe(pipe_pipe);

            fprintf(1, "%s\n", buffer);

            pipe_bytes--;
            character++;
            continue;
        };

        write(pipe_pipe[1], &pipe_input[character], 1);
        buffer_length++;
        pipe_bytes--;
        character++;
    };
*/
    /* for (int character = 0; character < pipe_bytes; character++)
     {
         // fprintf(1, "Tests: %c\n", pipe_input[character]);

         // Test for string definition charater
         // if (strcmp(pipe_input[character], '"') == 0) continue;
         if (pipe_input[character] == '"') continue;

         // fprintf(1, "Valid: %c\n", pipe_input[character]);

         write(pipe_pipe[1], &pipe_input[character], 1);
     };

     // Close the write end of the pipe
     close(pipe_pipe[1]);
     // Read the pipe out to the pipe pipe output
     read(pipe_pipe[0], pipe_output, 100);
     // Close the read of the pipe
     close(pipe_pipe[0]);

     fprintf(1, "Output: %s\n", pipe_output);

     // for (int i = 0; i < )

     // read(0, buffer, 1000);

     // fprintf(1, "%s\n", buffer);

     for (int i = 0; i < argc; i++)
     {
         fprintf(1, "%s\n", argv[i]);
     };*/
};