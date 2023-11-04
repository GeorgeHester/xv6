
Command*
parse_command(char* buffer)
{
    struct ExecuteCommand* p_command;
    char* p_buffer_end;
    // int value;

    p_command = create_execute_command();
    p_buffer_end = buffer + strlen(buffer);

    // fprintf(1, "%d\n", strlen(buffer));

    // char* buffer_copy;
    // buffer_copy = malloc(BUFFER_SIZE);
    //  memset(buffer_copy, 0, BUFFER_SIZE);
    // memcpy(buffer_copy, buffer, strlen(buffer));

    // fprintf(1, "%s\n", buffer);

    // fprintf(1, "%p\n", buffer);
    // fprintf(1, "%p\n", p_buffer_end);

    int argi = 0;
    char previous = 0;
    p_command->argv[argi] = buffer;

    while (buffer < p_buffer_end && argi < MAXIMUM_ARGUMENTS)
    {
        // Check for end of string reached
        if (buffer[0] == NULL) break;

        // Check if this is a split in the command
        if (buffer[0] == ' ')
        {
            if (previous == ' ')
            {
                buffer++;
                p_command->argv[argi] = buffer;
                continue;
            };

            previous = buffer[0];
            p_command->end_argv[argi] = buffer;
            argi++;
            buffer++;
            p_command->argv[argi] = buffer;
            continue;
        };

        previous = buffer[0];
        buffer++;
    };

    for (int i = 0; i < argi + 1; i++)
    {
        if (p_command->end_argv[i] != NULL)
        {
            *p_command->end_argv[i] = NULL;
        };

        // fprintf(1, "%p\n", p_command->argv[i]);
        //  fprintf(1, "%p\n", p_command->end_argv[i]);
    };

    // fprintf(1, "%s\n", buffer);

    /* for (int i = 0; i < argi + 1; i++)
     {
         fprintf(1, "%s\n", p_command->argv[i]);
     };*/

    /*while ( && strchr(whitespace, *buffer_copy))
        buffer_copy++;*/

    /*// char* p_buffer_end;
    struct ExecuteCommand* p_command;
    p_command = create_execute_command();

    // Get the pointer of the buffer that signifys the string terminating byte
    // p_buffer_end = buffer + strlen(buffer);

    // Pass a pointer to the buffer along with the pointer to the terminating
    // byte of the buffer
    // parse_line(&buffer, p_buffer_end);
    for (int buffer_index = 0; buffer_index < strlen(buffer); buffer_index++)
    {
        fprintf(1, "%c\n", buffer[buffer_index]);
    };


    return p_command;*/
    return (Command*)p_command;
};