#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

// Structure to hold command-line flags
struct Flags
{
    _Bool p;
    _Bool s;
    _Bool U;
    _Bool S;
    _Bool v;
    _Bool c;
};

void displayProcessInfo(const char *pid, struct Flags flags);

int main(int argc, char *argv[])
{
    struct Flags flags = {false, false, true, true, false, true}; // Default flags

    char *path = NULL;
    int opt;

    /*Parse through the command line if react to the flags
    accordingly.*/
    while ((opt = getopt(argc, argv, "p:sUSvc")) != -1)
    {
        switch (opt)
        {
        case 'p':
            path = optarg;
            flags.p = true;
            break;
        case 's':
            flags.s = true;
            break;
        case 'U':
            flags.U = false;
            break;
        case 'S':
            flags.S = true;
            break;
        case 'v':
            flags.v = true;
            break;
        case 'c':
            flags.c = false;
            break;
        default:
            fprintf(stderr, "Usage: %s -p PID -s -U -S -v -c\n", argv[0]);
            exit(0);
        }
    }

    // If no options are provided, display information for all processes of the current user
    if (!flags.p && !flags.s && !flags.U && !flags.S && !flags.c && !flags.v)
    {
        char currentUID[32];
        snprintf(currentUID, sizeof(currentUID), "%d", getuid());
        DIR *procDir = opendir("/proc");
        if (procDir)
        {
            struct dirent *entry;
            while ((entry = readdir(procDir)) != NULL)
            {
                if (isdigit(entry->d_name[0]) && strcmp(entry->d_name, currentUID) == 0)
                {
                    displayProcessInfo(entry->d_name, flags);
                }
            }
            closedir(procDir);
        }
        else
        {
            perror("procDir could not be opened.");
        }
    }
    else if (flags.p)
    {
        displayProcessInfo(path, flags);
    }

    return 0;
}

// Function to display process information
void displayProcessInfo(const char *pid, struct Flags flags)
{
    char procPath[256];
    snprintf(procPath, sizeof(procPath), "/proc/%s", pid);

    struct stat procStat;
    if (stat(procPath, &procStat) == 0)
    {
        printf("PID: %s\n", pid);

        if (flags.s)
        {
            char statusPath[256];
            snprintf(statusPath, sizeof(statusPath), "%s/status", procPath);
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile)
            {
                char line[256];
                while (fgets(line, sizeof(line), statusFile))
                {
                    if (strncmp(line, "State:", 6) == 0)
                    {
                        printf("%s", line);
                    }
                }
                fclose(statusFile);
            }
        }

        char statPath[256];
        snprintf(statPath, sizeof(statPath), "%s/stat", procPath);
        FILE *statFile = fopen(statPath, "r");
        if (statFile)
        {
            char line[256];
            if (fgets(line, sizeof(line), statFile))
            {
                char *token = strtok(line, " ");
                int count = 1;
                while (token)
                {
                    if (!flags.U && count == 14)
                    {
                        printf("User Time: %s\n", token);
                    }
                    else if (flags.S && count == 15)
                    {
                        printf("System Time: %s\n", token);
                    }
                    token = strtok(NULL, " ");
                    count++;
                }
            }
            fclose(statFile);
        }

        if (flags.v)
        {
            char statmPath[256];
            snprintf(statmPath, sizeof(statmPath), "%s/statm", procPath);
            FILE *statmFile = fopen(statmPath, "r");
            if (statmFile)
            {
                char line[256];
                if (fgets(line, sizeof(line), statmFile))
                {
                    char *token = strtok(line, " ");
                    if (token)
                    {
                        printf("Virtual Memory: %s pages\n", token);
                    }
                }
                fclose(statmFile);
            }
        }

        if (flags.c)
        {
            char cmdlinePath[256];
            snprintf(cmdlinePath, sizeof(cmdlinePath), "%s/cmdline", procPath);
            FILE *cmdlineFile = fopen(cmdlinePath, "r");
            if (cmdlineFile)
            {
                char cmdLine[256];
                if (fgets(cmdLine, sizeof(cmdLine), cmdlineFile))
                {
                    printf("Command Line: %s\n", cmdLine);
                }
                fclose(cmdlineFile);
            }
        }

        printf("\n");
    }
}