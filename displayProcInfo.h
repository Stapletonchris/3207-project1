#ifndef DISPLAYPROCINFO_H
#define DISPLAYPROCINFO_H

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

void displayProcInfo(const char *pid, struct Flags flags);

#endif