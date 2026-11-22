/*
 * Palette animation of decorations (src/game/Graphics/Palette/PaletteAnim_*):
 * PaletteAnim_Create / _Main / _Release / _Destructor and ObjDraw_LoadPaletteRow.
 *
 * On the DS a PaletteAnim task rewrites one OBJ palette row (the row
 * StageObj_AllocSpritePalette gave the decoration type) with the palette of
 * animation seq[frame].anim of the decoration's BAC, every seq[frame].time
 * frames; every sprite drawn with that row changes colour. Natively the
 * converted atlases are PAL4 with the DS indices (tools/dreamcast/sprites,
 * palettes.bin anim_pal[]), so the step is a palette-bank selection: a sprite
 * started here (ObjSprite.palanim) draws its row-0 images with the bank of
 * anim_pal[obj_palanim_anim()] (obj_draw.c). Nothing is re-expanded.
 *
 * One task per sequence table (gPaletteAnimTasks[row]: each table belongs to
 * one decoration type, so to one palette row); the reference count is the
 * number of live decorations (gPaletteAnimRefCount, PaletteAnim_Release from
 * StageObj_Destructor with flags 0x800). Tables: palanim.bin (tools/objdata.py).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj_internal.h"
#include "obj_zone.h"

#define PALANIM_MAX 8
#define PALANIM_SEQ 64
typedef struct {
    u32 addr;
    u16 n;
    u16 col[PALANIM_SEQ];
    u8 time[PALANIM_SEQ], flags[PALANIM_SEQ];
} PalSeq;
static struct {
    PalSeq seq[PALANIM_MAX];
    int n_seq;
    struct {
        u16 refs;      /* gPaletteAnimRefCount[idx]  */
        u8 alive;      /* the task exists            */
        u16 frame;     /* work.frame                 */
        u16 timer;     /* work.timer                 */
        s16 anim;      /* animation whose palette the row holds (-1: the sprite's own) */
    } st[PALANIM_MAX];
} s_pa;

static u16 rd16p(const u8 *p) { return (u16)(p[0] | p[1] << 8); }
static u32 rd32p(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }

int obj_palanim_load(const char *obj_dir) {
    char path[160];
    u8 *b;
    u32 size, o = 8;
    int i, k;
    memset(&s_pa, 0, sizeof(s_pa));
    snprintf(path, sizeof(path), "%s/palanim.bin", obj_dir);
    if (obj_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 8 || memcmp(b, "RPAL", 4) != 0 || rd16p(b + 4) != 1) {
        free(b);
        return -1;
    }
    for (i = 0; i < rd16p(b + 6) && s_pa.n_seq < PALANIM_MAX; i++) {
        PalSeq *q = &s_pa.seq[s_pa.n_seq];
        if (o + 8 > size)
            break;
        q->addr = rd32p(b + o);
        q->n = rd16p(b + o + 4);
        o += 8;
        if (q->n == 0 || q->n > PALANIM_SEQ || o + 4u * q->n > size)
            break;
        for (k = 0; k < q->n; k++) {
            q->col[k] = rd16p(b + o + 4 * k);
            q->time[k] = b[o + 4 * k + 2];
            q->flags[k] = b[o + 4 * k + 3];
        }
        o += 4u * q->n;
        s_pa.n_seq++;
    }
    free(b);
    for (i = 0; i < PALANIM_MAX; i++)
        s_pa.st[i].anim = -1;
    return s_pa.n_seq > 0 ? 0 : -1;
}

/* every decoration is gone (objects_shutdown / objects_init): no task survives */
void obj_palanim_reset(void) {
    int i;
    for (i = 0; i < PALANIM_MAX; i++) {
        memset(&s_pa.st[i], 0, sizeof(s_pa.st[i]));
        s_pa.st[i].anim = -1;
    }
}

/* PaletteAnim_Create(sprite row, table, res, NULL) for decoration w */
void obj_palanim_start(Obj *w, u32 table, s32 type) {
    int i;
    z_fxhook(NULL, OBJ_FX_PALETTE_ANIM, (s32)table, type);
    for (i = 0; i < s_pa.n_seq; i++)
        if (s_pa.seq[i].addr == table)
            break;
    if (i == s_pa.n_seq)
        return;
    w->spr.palanim = (u8)(i + 1);
    if (s_pa.st[i].refs != 0) {
        /* a running task: only a sequence without an end marker restarts (none in the ROM) */
        const PalSeq *q = &s_pa.seq[i];
        int k;
        s_pa.st[i].refs++;
        for (k = 0; k < q->n; k++) {
            if (q->flags[k] & 1)
                return;
            if (q->time[k] == 0)
                break;
        }
        s_pa.st[i].frame = 0;
        s_pa.st[i].timer = q->time[0];
        s_pa.st[i].anim = (s16)q->col[0];
        return;
    }
    s_pa.st[i].alive = 1;
    s_pa.st[i].refs = 1;
    s_pa.st[i].frame = 0;
    s_pa.st[i].timer = s_pa.seq[i].time[0];
    s_pa.st[i].anim = -1; /* the row keeps what StageObj_AllocSpritePalette loaded */
}

/* PaletteAnim_Release (StageObj_Destructor) */
void obj_palanim_release(Obj *w) {
    int i = w->spr.palanim - 1;
    if (i < 0 || i >= PALANIM_MAX)
        return;
    if (s_pa.st[i].refs != 0)
        s_pa.st[i].refs--;
    w->spr.palanim = 0;
}

/* PaletteAnim_Main of every task, once per frame */
void obj_palanim_main(void) {
    int i;
    for (i = 0; i < s_pa.n_seq; i++) {
        const PalSeq *q = &s_pa.seq[i];
        if (!s_pa.st[i].alive)
            continue;
        if (s_pa.st[i].refs == 0) { /* PaletteAnim_Main: Task_Destroy */
            s_pa.st[i].alive = 0;
            s_pa.st[i].anim = -1;
            continue;
        }
        if (s_pa.st[i].timer == 0) {
            s_pa.st[i].frame++;
            if (s_pa.st[i].frame >= q->n)
                s_pa.st[i].frame = 0;
            s_pa.st[i].timer = q->time[s_pa.st[i].frame];
            if (q->flags[s_pa.st[i].frame] & 1) {
                s_pa.st[i].frame = 0;
                s_pa.st[i].timer = q->time[0];
            }
            s_pa.st[i].anim = (s16)q->col[s_pa.st[i].frame]; /* ObjDraw_LoadPaletteRow x2 */
            if (s_pa.st[i].timer == 0) {
                s_pa.st[i].alive = 0;
                continue;
            }
        }
        if (s_pa.st[i].timer != 0)
            s_pa.st[i].timer--;
    }
}

/* the animation whose palette a sprite's row holds now, -1 = its own palette */
int obj_palanim_anim(int palanim) {
    int i = palanim - 1;
    if (i < 0 || i >= PALANIM_MAX)
        return -1;
    return s_pa.st[i].anim;
}
