/*MYps retrieves and displays process information
on a Linux system based on specified command-line flags.
It defines a structure, Flags, to hold these flags.
The code parses command-line arguments using getopt(),
sets default flag values, and processes user-defined flags.
The program can display information about processes,
including their state, user time, system time, virtual memory,
 and command lines, based on the user's flag preferences.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include "readproc.h"

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

void displayProcInfo(const char *pid, struct Flags flags);

int main(int argc, char *argv[])
{
    struct Flags flags = {false, false, true, true, false, true}; // Default flags

    char *path = NULL;
    int opt;

    // Parse through the command line and react to the flags accordingly.
    while ((opt = getopt(argc, argv, "p:sUSvc")) != -1)
    {
        switch (opt)
        {
        case 'p':
            path = optarg;
            flags.p = true; // Set the '-p' flag to indicate a specific PID is provided
            break;
        case 's':
            flags.s = true; // Set the '-s' flag to indicate displaying process state
            break;
        case 'U':
            flags.U = false; // Set the '-U' flag to indicate not displaying user time
            break;
        case 'S':
            flags.S = true; // Set the '-S' flag to indicate displaying system time
            break;
        case 'v':
            flags.v = true; // Set the '-v' flag to indicate displaying virtual memory
            break;
        case 'c':
            flags.c = false; // Set the '-c' flag to indicate not displaying the command line
            break;
        default:
            fprintf(stderr, "Usage: %s -p PID -s -U -S -v -c\n", argv[0]);
            exit(0);
        }
    }

    // If no options are provided, display information for all processes of the current user
    if (!flags.p && !flags.s && !flags.U && !flags.S && !flags.c && !flags.v)
    {
        // readproc();
        char currentUID[32];
        snprintf(currentUID, sizeof(currentUID), "%d", getuid());
        DIR *procDir = opendir("/proc");
        if (procDir)
        {
            struct dirent *entry;
            while ((entry = readdir(procDir)) != NULL)
            {
                if (isdigit(entry->d_name[0]))
                {
                    displayProcInfo(entry->d_name, flags);
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
        displayProcInfo(path, flags);
    }

    return 0;
}

// Function to display process information
void displayProcInfo(const char *pid, struct Flags flags)
{
    char procPath[256];
    /*Opens /proc uses PID to select the path*/
    snprintf(procPath, sizeof(procPath), "/proc/%s", pid);

    struct stat procStat;
    /*If the directory existist display
    retrieve its info*/
    if (stat(procPath, &procStat) == 0)
    {
        printf("PID: %s\n", pid);

        /*Display the single-character state information about the process.
        This information is found in the stat file in the process’s directory,
         by looking at the third (“state”) field.*/
        if (flags.s)
        {
            char statusPath[271];
            /*Opens the status file reads through, looks for
            lines starting with state. If it finds the first instance,
            print the line where it was found.*/
            snprintf(statusPath, sizeof(statusPath), "%s/status", procPath);
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile)
            {
                char line[269];
                /*Opens the /stat file, read line by line,
                split it by spaces count the spaces so that you know
                which index the token is at. if the U flag isn't present
                print the user time where the token was found.*/
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

        char statPath[269];
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
                fclose(statFile);
            }
        }

        if (flags.v)
        {
            char statmPath[271];
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
            char cmdlinePath[272];
            snprintf(cmdlinePath, sizeof(cmdlinePath), "%s/cmdline", procPath);
            FILE *cmdlineFile = fopen(cmdlinePath, "r");
            if (cmdlineFile)
            {
                char cmdLine[264];
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
