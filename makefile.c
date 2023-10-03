// #include <hellomake.h>

CC = gcc
    CFLAGS = -Wall - Wextra

                         TARGET = cpuinfo_cache

                                      all : $(TARGET)

                                                $(TARGET) : cpuinfo_cache.c
                                                            $(CC) $(CFLAGS) -
                                  o $(TARGET) cpuinfo_cache.c

                                      clean : rm -
                                  f $(TARGET)
