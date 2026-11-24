/* Simulation snapshots, see snap.h. */
#include <stdlib.h>
#include <string.h>
#include "snap.h"
#include "../../objects/obj_internal.h"

extern char vsr_snap_d0, vsr_snap_d1, vsr_snap_b0, vsr_snap_b1;
typedef struct { unsigned char *p; size_t n; } Reg;
static Reg R[6];
static int NR;
static unsigned *base, *cur;
static size_t NW;

static void add(void *p, size_t n) { if (p && n) { R[NR].p = (unsigned char *)p; R[NR].n = n; NR++; } }
static void gather(unsigned *dst) {
    unsigned char *d = (unsigned char *)dst;
    int i;
    for (i = 0; i < NR; i++) { memcpy(d, R[i].p, R[i].n); d += R[i].n; }
}
static void scatter(const unsigned *src) {
    const unsigned char *s = (const unsigned char *)src;
    int i;
    for (i = 0; i < NR; i++) { memcpy(R[i].p, s, R[i].n); s += R[i].n; }
}
int snap_init(void) {
    size_t n = 0;
    int i;
    char *d0 = &vsr_snap_d0, *d1 = &vsr_snap_d1, *b0 = &vsr_snap_b0, *b1 = &vsr_snap_b1;
    if (d1 <= d0 || b1 <= b0) return -1;
    NR = 0;
    add(d0, (size_t)(d1 - d0));
    add(b0, (size_t)(b1 - b0));
    add(g_obj.prm, sizeof(ObjParam) * (g_obj.hdr->num_objects ? g_obj.hdr->num_objects : 1));
    add(g_obj.ring_state, g_obj.hdr->num_rings ? g_obj.hdr->num_rings : 1);
    add(g_obj.dprm, sizeof(ObjParam) * g_obj.hdr->num_decor);
    for (i = 0; i < NR; i++) n += R[i].n;
    NW = (n + 3) / 4;
    base = (unsigned *)calloc(NW, 4);
    cur = (unsigned *)calloc(NW, 4);
    if (!base || !cur) return -1;
    gather(base);
    return 0;
}
size_t snap_bytes(void) { return NW * 4; }
void snap_take(Snap *s) {
    size_t i, k = 0;
    gather(cur);
    for (i = 0; i < NW; i++) if (cur[i] != base[i]) k++;
    s->w = (unsigned *)malloc(sizeof(unsigned) * 2 * (k ? k : 1));
    s->n = (unsigned)k;
    for (i = 0, k = 0; i < NW; i++)
        if (cur[i] != base[i]) { s->w[2 * k] = (unsigned)i; s->w[2 * k + 1] = cur[i]; k++; }
}
void snap_restore(const Snap *s) {
    unsigned i;
    memcpy(cur, base, NW * 4);
    for (i = 0; i < s->n; i++) cur[s->w[2 * i]] = s->w[2 * i + 1];
    scatter(cur);
}
void snap_free(Snap *s) { free(s->w); s->w = NULL; s->n = 0; }
