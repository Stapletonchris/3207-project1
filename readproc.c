#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define maxPath 512

void readproc()
{
    DIR *directory;
    struct dirent *entry;
    char path[maxPath];

    directory = opendir("/proc");

    if (directory == NULL)
    {
        perror("Error: couldn't open proc");
        exit(1);
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_DIR && strspn(entry->d_name, "0123456789") == strlen(entry->d_name))
        {
            snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);

            FILE *status_file = fopen(path, "r");
            if (status_file != NULL)
            {
                char line[256];
                char name[256];

                while (fgets(line, sizeof(line), status_file))
                {
                    if (sscanf(line, "Name:\t%s", name) == 1)
                    {
                        printf("PID: %s\n", entry->d_name);
                        printf("Name: %s\n", name);
                    }
                }
                fclose(status_file);
            }
        }
    }
    closedir(directory);
}
