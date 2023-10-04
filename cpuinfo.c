#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int opt;
    char *path = NULL;

    // Use getopt to parse command line options
    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            path = optarg;
            break;
        default:
            fprintf(stderr, "Usage: -f /proc/cpuinfo\n");
            exit(1);
        }
    }

    if (path == NULL)
    {
        fprintf(stderr, "You must provide the -f falg with /proc/cpuinfo\n");
        exit(1);
    }

    // More of a personal test
    if (strcmp(path, "/proc/cpuinfo") != 0)
    {
        fprintf(stderr, "Invalid file path. Use /proc/cpuinfo\n");
        exit(1);
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening /proc/cpuinfo");
        return 1;
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, file)) != -1)
    {
        // Find the "cache size" string in each line
        char *cache_size = strstr(line, "cache size");

        if (cache_size != NULL)
        {
            // Tokenize the line using ':' as the delimiter
            char *token = strtok(line, ":");

            // Find and print the cache size information
            while (token != NULL)
            {
                token = strtok(NULL, ":");
                if (token != NULL)
                {
                    printf("Cache Size: %s\n", token);
                }
            }
        }
    }

    // Close the file
    fclose(file);

    // Free allocated memory for getline
    if (line != NULL)
    {
        free(line);
    }

    return 0;
}
