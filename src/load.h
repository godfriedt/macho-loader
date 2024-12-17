#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <mach/mach_vm.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach/machine.h>

void loadcmds(FILE* file, int count, int total_size);
void p_hex(const char* data, int len);

#endif