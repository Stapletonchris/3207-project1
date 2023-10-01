#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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
            fprintf(stderr, "Usage: %s -f /proc/cpuinfo\n", argv[0]);
            exit(0);
        }
    }

    if (path == NULL)
    {
        fprintf(stderr, "You must provide the -f option with /proc/cpuinfo\n");
        exit(0);
    }

    if (strcmp(path, "/proc/cpuinfo") != 0)
    {
        fprintf(stderr, "Invalid file path. Use /proc/cpuinfo\n");
        exit(0);
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening /proc/cpuinfo");
        return 1;
    }

    char line[300];
    while (fgets(line, sizeof(line), file))
    {
        // Process each line in /proc/cpuinfo here
        printf("%s", line);
    }

    // If the file can be closed, close it
    if ((fclose(file)) != 0)
    {
        perror("Error closing /proc/cpuinfo\n");
    }
    return 0;
}
