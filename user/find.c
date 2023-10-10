#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/**
 * Function to check a certain directory for a file with a name matching the
 * query, prints the path to each match to the console
 */
int
check_directory(char* directory, char* query)
{
    int directory_descriptor;
    struct stat directory_data;
    struct dirent directory_entry;

    // Attempt to open the directory
    directory_descriptor = open(directory, O_RDONLY);
    if (directory_descriptor < 0) return 0;

    // Check if we can retrieve the file stats for the current directory
    if (fstat(directory_descriptor, &directory_data) < 0)
    {
        close(directory_descriptor);
        return 0;
    };

    // Check the type of the file stats
    switch (directory_data.type)
    {
        case T_DIR:
            // Loop through the file directory entries inside of the file
            // descriptor
            while (read(directory_descriptor,
                        &directory_entry,
                        sizeof(directory_entry)) == sizeof(directory_entry))
            {
                // Free directory check
                if (directory_entry.inum == 0) continue;

                // System directory check
                if (strcmp(directory_entry.name, ".") == 0) continue;
                if (strcmp(directory_entry.name, "..") == 0) continue;

                // Define path variable of correct size to hold full path
                char path[strlen(directory) + 15];

                // Create path variable pipe
                int path_pipe[2];
                pipe(path_pipe);

                // Write the current directory to the pipe, then write in the
                // delimiter and the entries name
                write(path_pipe[1], directory, strlen(directory));
                write(path_pipe[1], "/", 1);
                write(path_pipe[1], directory_entry.name, 14);

                // Close the write end of the pipe
                close(path_pipe[1]);
                // Read the pipe to the path variable
                read(path_pipe[0], path, strlen(directory) + 15);
                // Close the read end of the pipe
                close(path_pipe[0]);

                // Create variable to store the statistics about the entry
                struct stat directory_entry_data;

                // Attempt to load the statistics for the current entry
                if (stat(path, &directory_entry_data) < 0) continue;

                // Check current entry type
                switch (directory_entry_data.type)
                {
                    case T_FILE:
                        // Check for match
                        if (strcmp(directory_entry.name, query) == 0)
                            fprintf(1, "Match: %s\n", path);

                        break;
                    case T_DIR:
                        check_directory(path, query);

                        break;
                };
            };
            break;
        default:
            break;
    };

    close(directory_descriptor);
    return 0;
};

int
main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(1, "Usage: find <name>\n");
        exit(0);
    };

    check_directory(".", argv[1]);
    exit(0);
};