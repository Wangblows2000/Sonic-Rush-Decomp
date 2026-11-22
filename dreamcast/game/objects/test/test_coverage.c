/*
 * Coverage of the translated objects on the real levels: every placed object
 * and decoration entry of each stage is passed to its create function (as
 * StageEvent would) and counted as translated when the object comes up.
 * MapObj40 trigger types whose OnHit case needs untranslated code are
 * counted as not translated; PlayerStart (no object by design) counts as
 * translated, and so do the load-time forms of MapObj70 / WaterLevelTrigger.
 *
 * Usage: test_coverage <player tables dir> <objdata dir> <sprites bac dir> <stages dir> <stage id>...
 * Output: one markdown table row per stage, then the missing classes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../objects.h"
#include "../obj_internal.h"

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*size);
    if (fread(b, 1, *size, f) != (size_t)*size) {
        fclose(f);
        free(b);
        return NULL;
    }
    fclose(f);
    return b;
}

static int mapobj40_untranslated(u16 t) {
    return t == 0xb6 || t == 0xb7 || (t >= 0xc3 && t <= 0xc5); /* v3: 0x7e and 0xa9 translated */
}

static void flush(void) {
    Obj *o;
    int n;
    for (n = 0; n < 3; n++) {
        for (o = g_obj.head; o; o = o->next)
            o->h.obj_flags |= 4;
        objects_update(NULL);
    }
}

int main(int argc, char **argv) {
    char path[512];
    long size;
    s16 *trig;
    int a;
    static int miss_obj[0x100], miss_dec[0x100];
    if (argc < 6) {
        printf("usage: %s <player tables> <objdata dir> <sprites dir> <stages dir> <stage>...\n", argv[0]);
        return 2;
    }
    snprintf(path, sizeof(path), "%s/trig.bin", argv[1]);
    trig = (s16 *)load(path, &size);
    if (!trig)
        return 2;
    dcp_set_trig(trig, trig + 8192);
    printf("| stage | object types | object entries | decoration types | decoration entries |\n|---|---|---|---|---|\n");
    for (a = 5; a < argc; a++) {
        static rlvl_map_t M;
        u8 *lv;
        const rlvl_header_t *h;
        u8 tseen[0x100], tok[0x100], dseen[0x100], dok[0x100];
        u32 i, oi = 0, oi_ok = 0, di = 0, di_ok = 0;
        int nt = 0, nt_ok = 0, nd = 0, nd_ok = 0, t;
        snprintf(path, sizeof(path), "%s/%s/level.bin", argv[4], argv[a]);
        lv = (u8 *)load(path, &size);
        if (!lv || rlvl_bind(&M, lv) != 0)
            continue;
        h = (const rlvl_header_t *)lv;
        memset(&g_dcp_world, 0, sizeof(g_dcp_world));
        g_dcp_world.zone = h->zone;
        g_dcp_world.act = h->act;
        g_dcp_world.timescale = 0x100;
        g_dcp_world.objmgr = 0x18;
        objects_set_paths(argv[2], argv[3]);
        if (objects_init(&M, lv) != 0) {
            printf("| %s | objects_init failed | | | |\n", argv[a]);
            continue;
        }
        memset(tseen, 0, sizeof(tseen));
        memset(tok, 1, sizeof(tok));
        memset(dseen, 0, sizeof(dseen));
        memset(dok, 1, sizeof(dok));
        flush();
        for (i = 0; i < h->num_objects; i++) {
            const rlvl_object_t *e = &g_obj.lv_obj[i];
            ObjParam p;
            ObjCreateFn fn = obj_create_fn(e->type);
            Obj *r = NULL;
            int ok;
            memset(&p, 0, sizeof(p));
            p.x0 = (u8)e->x;
            p.y0 = (u8)e->y;
            p.type = e->type;
            p.flags = e->flags;
            p.p6 = e->p0;
            p.p7 = e->p1;
            p.p8 = e->p2;
            p.p9 = e->p3;
            p.ax = e->x;
            p.ay = e->y;
            if (fn)
                r = fn(&p, e->x << 8, e->y << 8);
            /* PlayerStart and the "apply at load" forms of MapObj70 / WaterLevelTrigger
             * set their state and return no object, as in the original */
            ok = (r != NULL || e->type == 0x72 || (e->type == 0x70 && (e->flags & 0x10)) ||
                  (e->type == 0x7c && (e->flags & 4))) &&
                 !mapobj40_untranslated(e->type);
            /* flapper / pawn variants refused for "save option" reasons do not count as missing */
            oi++;
            oi_ok += ok;
            tseen[e->type & 0xff] = 1;
            if (!ok) {
                tok[e->type & 0xff] = 0;
                miss_obj[e->type & 0xff]++;
            }
            flush();
        }
        for (i = 0; i < h->num_decor && g_obj.lv_dec; i++) {
            const rlvl_decor_t *e = &g_obj.lv_dec[i];
            ObjParam p;
            ObjDecCreateFn fn = e->type < 0xf2 ? obj_dec_create_fn(e->type) : NULL;
            Obj *r = NULL;
            memset(&p, 0, sizeof(p));
            p.x0 = (u8)e->x;
            p.y0 = (u8)e->y;
            p.type = e->type;
            if (fn)
                r = fn(&p, e->x << 8, e->y << 8);
            di++;
            di_ok += r != NULL;
            dseen[e->type & 0xff] = 1;
            if (r == NULL) {
                dok[e->type & 0xff] = 0;
                miss_dec[e->type & 0xff]++;
            }
            flush();
        }
        for (t = 0; t < 0x100; t++) {
            if (tseen[t]) {
                nt++;
                nt_ok += tok[t];
            }
            if (dseen[t]) {
                nd++;
                nd_ok += dok[t];
            }
        }
        printf("| %s | %d/%d | %u/%u | %d/%d | %u/%u |\n", argv[a], nt_ok, nt, oi_ok, oi, nd_ok, nd, di_ok, di);
        objects_shutdown();
        free(lv);
    }
    printf("\nnot translated (entries over all stages):\n  objects:");
    for (a = 0; a < 0x100; a++)
        if (miss_obj[a])
            printf(" %#x:%d", a, miss_obj[a]);
    printf("\n  decorations:");
    for (a = 0; a < 0x100; a++)
        if (miss_dec[a])
            printf(" %#x:%d", a, miss_dec[a]);
    printf("\n");
    return 0;
}
