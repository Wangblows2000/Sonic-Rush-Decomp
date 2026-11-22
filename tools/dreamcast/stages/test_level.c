/*
 * test_level.c - host check of level_format.h against the DS transcription.
 *
 *   gcc -O2 -Wall -o /tmp/stages/test_level tools/dreamcast/stages/test_level.c
 *   /tmp/stages/test_level build/dreamcast/stages/z11
 *
 * Reads <dir>/level.bin and <dir>/testvec.bin (written by stage2dc.py
 * --testvec N: queries evaluated by dscoll.py on the raw DS files) and
 * re-runs every query through the rlvl_* reference functions.  Also checks
 * header/section sanity and the object/ring/decor region tables.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "level_format.h"

static void *load(const char *path, long *size)
{
    FILE *f = fopen(path, "rb");
    void *p;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = malloc(*size + 1);
    if (fread(p, 1, *size, f) != (size_t)*size) {
        fclose(f);
        free(p);
        return NULL;
    }
    fclose(f);
    return p;
}

#pragma pack(push, 1)
typedef struct {
    int32_t x, y;
    uint8_t flags, dir, ain, tin;
    int8_t hh; uint8_t ah, th;
    int8_t hw; uint8_t aw, tw;
    int8_t pl; uint8_t apl, tpl;
} vec_t;
#pragma pack(pop)

int main(int argc, char **argv)
{
    char path[1024];
    long lsize, vsize;
    uint8_t *lvl;
    vec_t *v;
    rlvl_map_t m;
    long n, i, bad_h = 0, bad_w = 0, bad_p = 0, bad_struct = 0;
    int s;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <stage dir>\n", argv[0]);
        return 2;
    }
    snprintf(path, sizeof path, "%s/level.bin", argv[1]);
    lvl = load(path, &lsize);
    if (!lvl || rlvl_bind(&m, lvl) != 0) {
        fprintf(stderr, "bad level.bin\n");
        return 1;
    }
    if (sizeof(rlvl_header_t) != m.hdr->header_size || sizeof(rlvl_object_t) != 12) {
        fprintf(stderr, "struct size mismatch: header %zu vs %u, object %zu\n",
                sizeof(rlvl_header_t), m.hdr->header_size, sizeof(rlvl_object_t));
        return 1;
    }
    for (s = 0; s < RLVL_SEC_COUNT; s++) {
        const rlvl_section_t *sec = &m.hdr->sec[s];
        if (sec->offset % 32 || sec->offset + sec->size > (uint32_t)lsize)
            bad_struct++;
    }
    {
        uint32_t nreg = (uint32_t)m.hdr->region_cols * m.hdr->region_rows;
        const uint32_t *orf = rlvl_section(lvl, RLVL_SEC_OBJ_REGION, NULL);
        const uint32_t *rrf = rlvl_section(lvl, RLVL_SEC_RING_REGION, NULL);
        const uint32_t *drf = rlvl_section(lvl, RLVL_SEC_DECOR_REGION, NULL);
        const rlvl_object_t *ob = rlvl_section(lvl, RLVL_SEC_OBJECTS, NULL);
        uint32_t r, k;
        if (orf[nreg] != m.hdr->num_objects || rrf[nreg] != m.hdr->num_rings || drf[nreg] != m.hdr->num_decor)
            bad_struct++;
        /* every object lies inside its region */
        for (r = 0; r < nreg; r++)
            for (k = orf[r]; k < orf[r + 1]; k++)
                if (ob[k].x / 256 != r % m.hdr->region_cols || ob[k].y / 256 != r / m.hdr->region_cols ||
                    ob[k].type >= 0xF2)
                    bad_struct++;
        /* every block id in range */
        for (k = 0; k < (uint32_t)m.hdr->width_blocks * m.hdr->height_blocks; k++)
            if (m.layer[0][k] >= m.hdr->num_blocks || m.layer[1][k] >= m.hdr->num_blocks)
                bad_struct++;
    }

    snprintf(path, sizeof path, "%s/testvec.bin", argv[1]);
    v = load(path, &vsize);
    if (!v) {
        fprintf(stderr, "no testvec.bin\n");
        return 1;
    }
    n = vsize / (long)sizeof(vec_t);
    for (i = 0; i < n; i++) {
        uint8_t a = v[i].ain, t = v[i].tin;
        int32_t h = rlvl_tile_height(&m, v[i].y, v[i].x, v[i].flags, &a, &t);
        if ((int8_t)h != v[i].hh || a != v[i].ah || t != v[i].th) {
            if (bad_h < 5)
                printf("height mismatch #%ld (%d,%d f=%02x): got %d/%u/%u want %d/%u/%u\n", i, v[i].x, v[i].y,
                       v[i].flags, h, a, t, v[i].hh, v[i].ah, v[i].th);
            bad_h++;
        }
        a = v[i].ain; t = v[i].tin;
        h = rlvl_tile_width(&m, v[i].x, v[i].y, v[i].flags, &a, &t);
        if ((int8_t)h != v[i].hw || a != v[i].aw || t != v[i].tw) {
            if (bad_w < 5)
                printf("width mismatch #%ld (%d,%d f=%02x): got %d/%u/%u want %d/%u/%u\n", i, v[i].x, v[i].y,
                       v[i].flags, h, a, t, v[i].hw, v[i].aw, v[i].tw);
            bad_w++;
        }
        a = v[i].ain; t = v[i].tin;
        h = rlvl_probe_distance_long(&m, v[i].x, v[i].y, v[i].flags, v[i].dir, &a, &t);
        if ((int8_t)h != v[i].pl || a != v[i].apl || t != v[i].tpl) {
            if (bad_p < 5)
                printf("probe mismatch #%ld (%d,%d f=%02x d=%u): got %d/%u/%u want %d/%u/%u\n", i, v[i].x,
                       v[i].y, v[i].flags, v[i].dir, h, a, t, v[i].pl, v[i].apl, v[i].tpl);
            bad_p++;
        }
    }
    printf("%s: %ux%u blocks, %u blocks, %u objects, %u rings, %u decor; %ld vectors: "
           "height %ld, width %ld, probe_long %ld mismatches; struct errors %ld\n",
           argv[1], m.hdr->width_blocks, m.hdr->height_blocks, m.hdr->num_blocks, m.hdr->num_objects,
           m.hdr->num_rings, m.hdr->num_decor, n, bad_h, bad_w, bad_p, bad_struct);
    return (bad_h || bad_w || bad_p || bad_struct) ? 1 : 0;
}
