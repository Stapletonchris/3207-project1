#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define maxPath 512 // Increase the buffer size to 512

int main(int argc, char **argv)
{

    DIR *directory; // Initialize directory pointer

    struct dirent *entry; // Directory entries pointer
    char path[maxPath];   // Initialize variable to store file path

    directory = opendir("/proc"); // Opens the /proc directory

    if (directory == NULL)
    {
        perror("Error: couldn't open proc");
        exit(1);
    }

    /* Read through directory */
    while ((entry = readdir(directory)) != NULL)
    {

        // Check if the entry is a directory and contains only digits (likely a process directory)
        if (entry->d_type == DT_DIR && strspn(entry->d_name, "0123456789") == strlen(entry->d_name))
        {
            // If the entry is a directory with digits, construct the path to the process status file

            snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);

            // Open the status file
            FILE *status_file = fopen(path, "r");
            if (status_file != NULL)
            {

                char line[256];
                char name[256];

                // Read lines from the status file
                while (fgets(line, sizeof(line), status_file))
                {

                    // Check if the line contains the process name
                    if (sscanf(line, "Name:\t%s", name) == 1)
                    {
                        // Print the PID and process name
                        printf("PID: %s\n", entry->d_name);
                        printf("Name: %s\n", name);
                    }
                }
                fclose(status_file); // Close the status file
            }
        }
    }
    closedir(directory); // Close the directory

    return 0;
}
