/* Malloc tracking, measurement builds only (make MEMTRACK=1): engine/memtrack.c */
#ifndef SRDC_MEMTRACK_H
#define SRDC_MEMTRACK_H
/* frameprof_scene calls it (weak: absent in normal builds): at each start of the scene
 * named in /cd/debug/memtrack.txt, the live heap blocks per caller vs the previous visit */
void memtrack_scene(const char *name) __attribute__((weak));
/* a block a loader helper allocated for its caller (pak_load): tag it with that caller */
void memtrack_retag(void *p, void *caller) __attribute__((weak));
#endif
