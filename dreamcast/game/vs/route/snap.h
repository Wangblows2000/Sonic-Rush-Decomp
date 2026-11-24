/* Whole-simulation snapshots for the route search (host): every static and global of the
 * simulation sources (bracketed by snap_begin.o / snap_end.o in link order) and the object
 * module's mutable heap copies (event, ring and decoration state). Stored as the words that
 * differ from the image at snap_init. */
#ifndef SRDC_VS_SNAP_H
#define SRDC_VS_SNAP_H
#include <stddef.h>
typedef struct Snap { unsigned *w; unsigned n; } Snap;   /* (index, value) pairs */
int snap_init(void);                 /* after the simulation is set up: the base image */
void snap_take(Snap *s);
void snap_restore(const Snap *s);
void snap_free(Snap *s);
size_t snap_bytes(void);             /* the image size */
#endif
