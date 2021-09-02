/*
    Copyright (c) 2017-2020 ByteBit

    This file is part of BetterSpades.

    BetterSpades is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BetterSpades is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with BetterSpades.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <common.hpp>
#include <log.hpp>
#include <file.hpp>

struct fhandle {
    void* internal;
    int type;
};

enum {
    FILE_STD,
    FILE_SDL,
};

void file_url(char* url) {
    char cmd[strlen(url) + 16];
#ifdef OS_WINDOWS
    sprintf(cmd, "start %s", url);
    system(cmd);
#endif
#if defined(OS_LINUX) || defined(OS_APPLE)
    sprintf(cmd, "xdg-open %s", url);
    system(cmd);
#endif
}

int file_dir_exists(const char* path) {
    DIR* d = opendir(path);
    if(d) {
        closedir(d);
        return 1;
    } else {
        return 0;
    }
}

int file_dir_create(const char* path) {
#ifdef OS_WINDOWS
    mkdir(path);
#else
    mkdir(path, 0755);
#endif
    return 1;
}

int file_exists(const char* name) {
    return !access(name, F_OK);
}

int file_size(const char* name) {
    FILE* f = fopen(name, "rb");
    if(!f)
        return 0;
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fclose(f);
    return size;
}

unsigned char* file_load(const char* name) {
    FILE* f;
    f = fopen(name, "rb");
    if(!f) {
        log_fatal("ERROR: failed to open '%s', exiting", name);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    auto data = (unsigned char*) malloc(size + 1);
    CHECK_ALLOCATION_ERROR(data)
    data[size] = 0;
    fseek(f, 0, SEEK_SET);
    fread(data, size, 1, f);
    fclose(f);
    return data;
}

void* file_open(const char* name, const char* mode) {
    return fopen(name, mode);
}

void file_printf(void* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf((FILE*)file, fmt, args);
    va_end(args);
}

void file_close(void* file) {
    fclose((FILE*) file);
}

float buffer_readf(unsigned char* buffer, int index) {
    return ((float*)(buffer + index))[0];
}

unsigned int buffer_read32(unsigned char* buffer, int index) {
    return (buffer[index + 3] << 24) | (buffer[index + 2] << 16) | (buffer[index + 1] << 8) | buffer[index];
}

unsigned short buffer_read16(unsigned char* buffer, int index) {
    return (buffer[index + 1] << 8) | buffer[index];
}

unsigned char buffer_read8(unsigned char* buffer, int index) {
    return buffer[index];
}
