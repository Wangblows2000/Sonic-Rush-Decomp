
#include <nitro/types.h>

typedef struct {
    u32 l;
    u32 t;
    u32 r;
    u32 b;
} UnkRect_02334c18;

typedef struct {
    u8 x;
    u8 y;
    u16 type;
    u16 flags;
    u8 pad6[6];
} UnkEnt_02334c18;

typedef struct {
    u8 pad0[0x1c];
    u8 *map;
} UnkWork_02334c18;

typedef struct {
    u8 pad0[0xe];
    u16 unkE;
} UnkStruct_0236abd8;

typedef void (*UnkFunc_02334c18)(UnkEnt_02334c18 *e, s32 x, s32 y, u32 a);

extern UnkStruct_0236abd8 data_ov000_0236abd8;
extern u8 data_ov000_023686e0[];
extern u16 data_ov000_0236aa78;
extern UnkFunc_02334c18 gMapObjectCreateTable[];

void StageEvent_SpawnMapObjectsInBlock(UnkWork_02334c18 *w, u16 x, u16 y, UnkRect_02334c18 *rc, UnkRect_02334c18 *o, u32 f) {
    u32 px;
    UnkFunc_02334c18 fn;
    UnkEnt_02334c18 *e;
    s32 zt;
    s32 zb;
    s32 zl;
    u32 py;
    u8 *tbl;
    u32 z;
    u16 ml;
    u16 mb;
    u16 mt;
    s32 d;
    s32 zr;
    u16 n;
    u16 i;
    u8 *base;
    s32 bx;
    u16 mr;
    s32 by;
    u32 off;

    zl = 0;
    zt = 0;
    zr = 0;
    zb = 0;
    z = 0;
    base = w->map;
    off = *(u32 *)(base + 4 + (u16)(y * (((data_ov000_0236abd8.unkE - 1) >> 8) + 1) + x) * 4);
    bx = x << 8;
    e = (UnkEnt_02334c18 *)(base + off + 2);
    by = y << 8;
    n = *(u16 *)(base + off);
    tbl = data_ov000_023686e0;
    for (i = 0; i < n; i++, e++) {
        if ((f & 8) && !(e->flags & 0x4000))
            continue;
        if (e->x != 0xff) {
            px = (u32)e->x + bx;
            py = (u32)e->y + by;
            ml = (f & 0x10) ? zl : tbl[e->type];
            mt = (f & 0x20) ? zt : tbl[e->type];
            mr = (f & 0x40) ? zr : tbl[e->type];
            mb = (f & 0x80) ? zb : tbl[e->type];
            if (f & 1) {
                if (px < rc->l + ml || px > rc->r - mr || py < rc->t + mt || py > rc->b - mb)
                    continue;
            }
            if (f & 4) {
                d = (s32)(data_ov000_0236aa78 - tbl[e->type]) / 3;
                if (px > o->l - d && px < o->r + d && py > o->t - d && py < o->b + d)
                    continue;
            }
            if (e->type >= 0xf2)
                continue;
            fn = gMapObjectCreateTable[e->type];
            if (fn == NULL)
                continue;
            fn(e, (bx + e->x) << 8, (by + e->y) << 8, z);
        }
    }
}
