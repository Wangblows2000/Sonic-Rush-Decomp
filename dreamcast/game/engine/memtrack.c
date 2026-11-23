/*
 * Malloc tracking (measurement builds only): every live heap block with its size and the
 * code address that allocated it, and a per-caller comparison between two visits of the
 * same scene, to find heap growth that a long session shows.
 *
 * Build:  make MEMTRACK=1   (links engine/memtrack.o and wraps the allocator:
 *         -Wl,--wrap=malloc,--wrap=free,--wrap=calloc,--wrap=realloc,--wrap=memalign)
 * Use:    /cd/debug/memtrack.txt holds a frameprof scene name ("z13 sonic").  Each time
 *         that scene starts (frameprof_scene calls memtrack_scene), the live blocks are
 *         summed per caller and every caller whose block count or bytes changed since the
 *         previous visit is printed:
 *           memtrack z13 sonic: visit 3, 4211 blocks 2489312 B (+8 blocks +8160 B)
 *           memtrack   8c0412a6: 3 blocks 3072 B (+1 blocks +1024 B)
 *         Resolve the addresses with sh-elf-addr2line -f -e srdc.elf.
 * The caller is the return address of the allocator call: a block allocated inside newlib
 * (fopen's FILE buffer) is tagged with the libc address, not the game's.
 */
#include <kos.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memtrack.h"

void *__real_malloc(size_t n);
void __real_free(void *p);
void *__real_calloc(size_t n, size_t s);
void *__real_realloc(void *p, size_t n);
void *__real_memalign(size_t a, size_t n);

#define MT_SLOTS 32768            /* live blocks (open addressing, power of two) */
#define MT_CALLERS 1024
typedef struct { void *p; uint32_t size; void *caller; } MtSlot;
typedef struct { void *caller; uint32_t n, bytes; } MtCaller;
static MtSlot s_tab[MT_SLOTS];
static uint32_t s_live, s_lost;   /* blocks in the table; blocks not recorded (table full) */
static MtCaller s_prev[MT_CALLERS], s_cur[MT_CALLERS];
static int s_nprev, s_ncur, s_visits, s_inside;
static char s_scene[32];
static int s_init;

static uint32_t h_ptr(const void *p) { uint32_t x = (uint32_t)(uintptr_t)p >> 3; x ^= x >> 13; x *= 0x9e3779b1u; return x; }

static void mt_add(void *p, size_t n, void *caller)
{
    uint32_t i, k;
    if(!p) return;
    for(k = 0, i = h_ptr(p) & (MT_SLOTS - 1); k < MT_SLOTS; k++, i = (i + 1) & (MT_SLOTS - 1))
        if(s_tab[i].p == NULL || s_tab[i].p == (void *)1) {
            s_tab[i].p = p;
            s_tab[i].size = (uint32_t)n;
            s_tab[i].caller = caller;
            s_live++;
            return;
        }
    s_lost++;
}
static void mt_del(void *p)
{
    uint32_t i, k;
    if(!p) return;
    for(k = 0, i = h_ptr(p) & (MT_SLOTS - 1); k < MT_SLOTS && s_tab[i].p; k++, i = (i + 1) & (MT_SLOTS - 1))
        if(s_tab[i].p == p) {
            s_tab[i].p = (void *)1;   /* tombstone */
            s_live--;
            return;
        }
}

void memtrack_retag(void *p, void *caller)
{
    uint32_t i, k;
    int o;
    if(!p) return;
    o = irq_disable();
    for(k = 0, i = h_ptr(p) & (MT_SLOTS - 1); k < MT_SLOTS && s_tab[i].p; k++, i = (i + 1) & (MT_SLOTS - 1))
        if(s_tab[i].p == p) { s_tab[i].caller = caller; break; }
    irq_restore(o);
}

void *__wrap_malloc(size_t n)
{
    void *c = __builtin_return_address(0), *p = __real_malloc(n);
    int o;
    if(p && !s_inside) { o = irq_disable(); mt_add(p, n, c); irq_restore(o); }
    return p;
}
void *__wrap_calloc(size_t n, size_t s)
{
    void *c = __builtin_return_address(0), *p = __real_calloc(n, s);
    int o;
    if(p && !s_inside) { o = irq_disable(); mt_add(p, n * s, c); irq_restore(o); }
    return p;
}
void *__wrap_memalign(size_t a, size_t n)
{
    void *c = __builtin_return_address(0), *p = __real_memalign(a, n);
    int o;
    if(p && !s_inside) { o = irq_disable(); mt_add(p, n, c); irq_restore(o); }
    return p;
}
void *__wrap_realloc(void *q, size_t n)
{
    void *c = __builtin_return_address(0), *p = __real_realloc(q, n);
    int o;
    if(p || n == 0) {
        o = irq_disable();
        mt_del(q);
        if(!s_inside) mt_add(p, n, c);
        irq_restore(o);
    }
    return p;
}
void __wrap_free(void *p)
{
    int o;
    if(!p) return;
    o = irq_disable();   /* out of the table before the block can be handed out again */
    mt_del(p);
    irq_restore(o);
    __real_free(p);
}

static int by_caller(const void *a, const void *b)
{
    uintptr_t x = (uintptr_t)((const MtCaller *)a)->caller, y = (uintptr_t)((const MtCaller *)b)->caller;
    return x < y ? -1 : x > y;
}
static const MtCaller *find(const MtCaller *t, int n, void *caller)
{
    int lo = 0, hi = n;
    while(lo < hi) {
        int m = (lo + hi) / 2;
        if((uintptr_t)t[m].caller < (uintptr_t)caller) lo = m + 1; else hi = m;
    }
    return lo < n && t[lo].caller == caller ? &t[lo] : NULL;
}

void memtrack_scene(const char *name)
{
    uint32_t i, nb = 0, bytes = 0, pb = 0, pn = 0;
    int k, o;
    if(!s_init) {   /* the scene to compare: /cd/debug/memtrack.txt */
        FILE *f;
        s_init = 1;
        s_inside = 1;
        f = fopen("/cd/debug/memtrack.txt", "rb");
        if(f) {
            size_t r = fread(s_scene, 1, sizeof(s_scene) - 1, f);
            s_scene[r] = 0;
            while(r && (s_scene[r - 1] == '\n' || s_scene[r - 1] == '\r' || s_scene[r - 1] == ' ')) s_scene[--r] = 0;
            fclose(f);
            printf("memtrack: comparing visits of \"%s\"\n", s_scene);
        }
        s_inside = 0;
    }
    if(!name || !s_scene[0] || strcmp(name, s_scene)) return;
    /* per-caller sums of the live blocks */
    o = irq_disable();
    s_ncur = 0;
    for(i = 0; i < MT_SLOTS; i++) {
        MtSlot *e = &s_tab[i];
        if(!e->p || e->p == (void *)1) continue;
        nb++;
        bytes += e->size;
        for(k = 0; k < s_ncur && s_cur[k].caller != e->caller; k++) {}
        if(k == s_ncur) {
            if(s_ncur == MT_CALLERS) continue;
            s_cur[k].caller = e->caller;
            s_cur[k].n = s_cur[k].bytes = 0;
            s_ncur++;
        }
        s_cur[k].n++;
        s_cur[k].bytes += e->size;
    }
    irq_restore(o);
    qsort(s_cur, (size_t)s_ncur, sizeof(MtCaller), by_caller);
    for(k = 0; k < s_nprev; k++) { pb += s_prev[k].bytes; pn += s_prev[k].n; }
    s_visits++;
    s_inside = 1;
    printf("memtrack %s: visit %d, %lu blocks %lu B (%+ld blocks %+ld B), %lu not recorded\n", name, s_visits,
           (unsigned long)nb, (unsigned long)bytes, s_visits > 1 ? (long)nb - (long)pn : 0L,
           s_visits > 1 ? (long)bytes - (long)pb : 0L, (unsigned long)s_lost);
    if(s_visits > 1) {
        for(k = 0; k < s_ncur; k++) {   /* changed or new callers */
            const MtCaller *p = find(s_prev, s_nprev, s_cur[k].caller);
            if(!p || p->n != s_cur[k].n || p->bytes != s_cur[k].bytes)
                printf("memtrack   %08lx: %lu blocks %lu B (%+ld blocks %+ld B)\n", (unsigned long)(uintptr_t)s_cur[k].caller,
                       (unsigned long)s_cur[k].n, (unsigned long)s_cur[k].bytes, (long)s_cur[k].n - (long)(p ? p->n : 0),
                       (long)s_cur[k].bytes - (long)(p ? p->bytes : 0));
        }
        for(k = 0; k < s_nprev; k++)   /* callers whose blocks are all gone */
            if(!find(s_cur, s_ncur, s_prev[k].caller))
                printf("memtrack   %08lx: 0 blocks 0 B (-%lu blocks -%lu B)\n", (unsigned long)(uintptr_t)s_prev[k].caller,
                       (unsigned long)s_prev[k].n, (unsigned long)s_prev[k].bytes);
    }
    s_inside = 0;
    memcpy(s_prev, s_cur, sizeof(MtCaller) * (size_t)s_ncur);
    s_nprev = s_ncur;
}
