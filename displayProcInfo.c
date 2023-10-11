#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "flags.h"

// Function to display process information
void displayProcInfo(const char *pid, struct Flags flags)
{
    char procPath[264];
    // check
    snprintf(procPath, sizeof(procPath), "/proc/%s", pid);

    struct stat procStat;
    if (stat(procPath, &procStat) == 0)
    {
        printf("PID: %s\n", pid);

        if (flags.s)
        {
            char statusPath[271];
            // check
            snprintf(statusPath, sizeof(statusPath), "%s/status", procPath);
            /*We are now in and reading*/
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile)
            {
                char line[264];
                /*Read through each line if the first 6 charaters
                start with "State:" then print the line its at.*/
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

        /*Runs the logic for the U and S flags*/
        char statPath[269];
        // Check
        snprintf(statPath, sizeof(statPath), "%s/stat", procPath);
        FILE *statFile = fopen(statPath, "r");
        if (statFile)
        {
            char line[256];
            if (fgets(line, sizeof(line), statFile))
            {
                /*Split line by token, count keeps track of the position of token.*/
                char *token = strtok(line, " ");
                int count = 1;
                while (token)
                {
                    if (!flags.U)
                    {
                        printf("User Time: %s\n", token);
                    }
                    else if (flags.S)
                    {
                        printf("System Time: %s\n", token);
                    }
                    token = strtok(NULL, " ");
                    count++;
                }
            }
            fclose(statFile);
        }

        // Tracks the v flag
        if (flags.v)
        {
            char statmPath[271];
            // Check
            snprintf(statmPath, sizeof(statmPath), "%s/statm", procPath);
            FILE *statmFile = fopen(statmPath, "r");
            if (statmFile)
            {
                char line[256];
                if (fgets(line, sizeof(line), statmFile))
                {
                    /*Splits line by space " " into token, when the first
                    space is found print the string following where the
                    token was found.*/
                    char *token = strtok(line, " ");
                    if (token)
                    {
                        printf("Virtual Memory: %s pages\n", token);
                    }
                }
                fclose(statmFile);
            }
        }

        // Handles the c flagq
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
