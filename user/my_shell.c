/**
 * Custom shell for XV6
 */
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define NULL 0
#define BUFFER_SIZE 128
#define MAXIMUM_ARGUMENTS 16

char whitespace[6] = " \t\r\n\v";
char symbols[] = "<|>&;()";

void
error(char* message)
{
    fprintf(2, "%s\n", message);
    exit(1);
};

int
safe_fork(void)
{
    int process_id;
    process_id = fork();

    if (process_id == -1) error("Failed fork");

    return process_id;
};

enum CommandType
{
    execute_command,
    list_command,
    redirection_command,
    pipe_command
};

typedef struct Command
{
    enum CommandType type;
} Command;

typedef struct ExecuteCommand
{
    enum CommandType type;
    char* argv[MAXIMUM_ARGUMENTS];
    char* end_argv[MAXIMUM_ARGUMENTS];
} ExecuteCommand;

typedef struct ListCommand
{
    enum CommandType type;
    struct Command* p_left_command;
    struct Command* p_right_command;
} ListCommand;

typedef struct PipeCommand
{
    enum CommandType type;
    struct Command* p_left_command;
    struct Command* p_right_command;
} PipeCommand;

typedef struct RedirectionCommand
{
    enum CommandType type;
    struct Command* p_command;
    char* p_file_name;
    char* p_file_name_end;
    int file_mode;
    int file_descriptor;
} RedirectionCommand;

/**
 * Create and populate a redirection command structure
 */
ExecuteCommand*
create_execute_command(void)
{
    ExecuteCommand* command;

    command = malloc(sizeof(*command));
    memset(command, 0, sizeof(*command));
    command->type = execute_command;
    return command;
};

/**
 * Create and populate a list command structure
 */
ListCommand*
create_list_command(Command* p_left_command, Command* p_right_command)
{
    ListCommand* command;

    command = malloc(sizeof(*command));
    memset(command, NULL, sizeof(*command));
    command->type = list_command;
    command->p_left_command = p_left_command;
    command->p_right_command = p_right_command;
    return command;
};

/**
 * Create and populate a pipe command structure
 */
PipeCommand*
create_pipe_command(Command* p_left_command, Command* p_right_command)
{
    PipeCommand* command;

    command = malloc(sizeof(*command));
    memset(command, NULL, sizeof(*command));
    command->type = pipe_command;
    command->p_left_command = p_left_command;
    command->p_right_command = p_right_command;
    return command;
};

/**
 * Create and populate a redirection command structure
 */
RedirectionCommand*
create_redirection_command(Command* p_command,
                           char* p_file_name,
                           char* p_file_name_end,
                           int file_mode,
                           int file_descriptor)
{
    RedirectionCommand* command;

    command = malloc(sizeof(*command));
    memset(command, 0, sizeof(*command));
    command->type = redirection_command;
    command->p_command = p_command;
    command->p_file_name = p_file_name;
    command->p_file_name_end = p_file_name_end;
    command->file_mode = file_mode;
    command->file_descriptor = file_descriptor;
    return command;
};

/**
 * Check for a given set of tokens in the next part of the buffer
 */
int
find(char** p_buffer, char* p_buffer_end, char* tokens)
{
    // Get the value stored in the buffer pointer
    char* buffer;
    buffer = *p_buffer;

    // Make sure we haven't reached the end of the buffer and we have a
    // whitespace character left, if so increase the pointer to move to the next
    // character or byte
    while (buffer < p_buffer_end && strchr(whitespace, *buffer))
        buffer++;

    // Update the value of the buffer
    *p_buffer = buffer;

    // Check there is any data left in the buffer and that one of the tokens
    // inputted exists in the remaining buffer
    return *buffer && strchr(tokens, *buffer);
};

void
run_command(Command* command)
{
    ExecuteCommand* command_execute;

    if (command == NULL) exit(1);

    switch (command->type)
    {
        default:
            error("Failed run command");
            break;
        case execute_command:
            // Cast the command to the correct type
            command_execute = (ExecuteCommand*)command;

            // Check the command is not empty
            if (command_execute->argv[0] == NULL) exit(1);

            /*int argi = 0;
            while (command_execute->argv[0] != NULL)
            {
                fprintf(1, "DEBUG %s\n", command_execute->argv[argi]);
                argi++;
            };*/

            exec(command_execute->argv[0], command_execute->argv);
            fprintf(2, "Failed to execute %s\n", command_execute->argv[0]);
            break;
    };

    exit(0);
};

Command*
parse_command(char* buffer)
{
    if (strchr(buffer, ";"))
    {
        ExecuteCommand* p_command;
        p_command = create_execute_command();

        return (Command*)p_command;
    };

    if (strchr(buffer, "|"))
    {
        Command* p_left_command = parse_command(buffer);
        Command* p_right_command = parse_command(buffer);

        PipeCommand* p_command;
        p_command = create_pipe_command(p_left_command, p_right_command);
        p_command->type = pipe_command;

        return (Command*)p_command;
    };
};

Command*
parse_list(char* buffer){

    /*
    void
    parse_line(char** p_buffer, char* p_buffer_end){

    };

    struct Command*
    parse_execute(){

    };

    void
    parse_pipe(char** p_buffer, char* p_buffer_end)
    {
        struct Command* p_command;

        p_command = parse_execute();
        if (find(p_buffer, p_buffer_end, "|"))
        {

            // command =
        };
    };
    */
};

void
open_console_file_descriptor(void)
{
    int file_descriptor;

    while ((file_descriptor = open("console", O_RDWR)) >= 0)
    {
        if (file_descriptor >= 3)
        {
            close(file_descriptor);
            break;
        };
    };
};

/**
 * Get input from user, outputting the required prompt
 */
int
prompt_user(char* buffer)
{
    // Output the user prompt
    write(1, ">>> ", 4);
    // Set the buffer to NULL
    memset(buffer, NULL, BUFFER_SIZE);

    // Read data from standard input up to the size of the buffer
    gets(buffer, BUFFER_SIZE);

    // Test to make sure the buffer is not empty
    if (buffer[0] == NULL) return -1;
    return 0;
};

/**
 * Remove the whitespace from before the buffer data starts
 */
void
remove_prefixed_whitespace_buffer(char* buffer)
{
    while (strchr(whitespace, buffer[0]) != NULL)
    {
        memcpy(buffer, buffer + 1, strlen(buffer));
    };
};

/**
 * Terminate the final character of the buffer
 */
void
terminate_buffer(char* buffer)
{
    buffer[strlen(buffer) - 1] = NULL;
};

int
main(void)
{
    static char buffer[BUFFER_SIZE];

    open_console_file_descriptor();

    while (prompt_user(buffer) >= 0)
    {
        remove_prefixed_whitespace_buffer(buffer);
        terminate_buffer(buffer);

        // Copy first part of the buffer to compare for cd command
        char cd_test[4];
        memcpy(cd_test, &buffer, 3);
        cd_test[3] = 0;

        // fprintf(1, "DEBUG %d\n", strlen(buffer));

        buffer[strlen(buffer) - 1] = NULL;

        // write(1, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "out\n") == 0) exit(0);

        // Compare the cd test string to test for a cd command
        if (strcmp(cd_test, "cd ") == 0)
        {

            // Set the endpoint of the buffer string
            // buffer[strlen(buffer) - 1] = 0;
            // Execute the cd command
            int cd_success = chdir(buffer + 3);

            // Check for change directory success
            if (cd_success < 0) fprintf(2, "Failed cd %s\n", buffer + 3);
            continue;
        };

        if (safe_fork() == 0) run_command(parse_command(buffer));
        wait(0);
        // write(1, buffer, BUFFER_SIZE);
    };

    exit(0);
};

/*typedef struct ListCommand
{
    enum CommandType type;
    struct Command* left_command;
    struct Command* right_command;
} ListCommand;*/

/*typedef struct BackgroundCommand
{
    enum CommandType type;
    struct Command* command;
} BackgroundCommand;*/