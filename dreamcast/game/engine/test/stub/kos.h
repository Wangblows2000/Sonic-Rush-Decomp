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
/* interrupts and the ASIC event table (the palette writes at the render-done interrupt) */
int irq_disable(void);
void irq_restore(int old);
#define ASIC_EVT_PVR_RENDERDONE_TSP 0x0002
typedef void (*asic_evt_handler)(uint32_t code, void *data);
typedef struct { asic_evt_handler hdl; void *data; } asic_evt_handler_entry_t;
asic_evt_handler_entry_t asic_evt_set_handler(uint16_t code, asic_evt_handler handler, void *data);
#endif
