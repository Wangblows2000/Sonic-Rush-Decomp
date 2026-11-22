/* host stub of <kos.h> for engine/render.c (test_palette.c) */
#ifndef STUB_KOS_H
#define STUB_KOS_H
#include <dc/pvr.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
typedef int file_t;
#define FILEHND_INVALID (-1)
file_t fs_open(const char *p, int mode);
void fs_close(file_t f);
ssize_t fs_read(file_t f, void *b, size_t n);
size_t fs_total(file_t f);
#endif
