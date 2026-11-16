#include <nitro/types.h>

typedef struct {
    u32 l;
    u32 t;
    u32 r;
    u32 b;
} UnkRect_023348f0;

typedef struct {
    u8 x;
    u8 y;
    u16 type;
} UnkEnt_023348f0;

typedef struct {
    u8 pad0[0x24];
    u8 *map;
} UnkWork_023348f0;

typedef struct {
    u8 pad0[0xe];
    u16 unkE;
} UnkStruct_0236abd8;

typedef void (*UnkFunc_023348f0)(UnkEnt_023348f0 *e, s32 x, s32 y, u32 a);

extern UnkStruct_0236abd8 data_ov000_0236abd8;
extern u8 data_ov000_023685ec[];
extern UnkFunc_023348f0 gDecorationCreateTable[];

void StageEvent_SpawnDecorationsInBlock(UnkWork_023348f0 *w, u16 x, u16 y, UnkRect_023348f0 *rc, UnkRect_023348f0 *o, u32 f) {
    u32 off;
    UnkEnt_023348f0 *e;
    u16 n;
    u8 *base;
    u16 i;
    s32 bx;
    s32 by;
    u32 px;
    u32 py;
    u16 ml;
    u16 mt;
    u16 mr;
    u16 mb;
    UnkFunc_023348f0 fn;
    u8 *tbl;
    s32 zl;
    s32 zt;
    s32 zr;
    s32 zb;
    u32 z;

    zl = 0;
    zt = 0;
    zr = 0;
    zb = 0;
    z = 0;
    base = w->map;
    off = *(u32 *)(base + 4 + (u16)(y * (((data_ov000_0236abd8.unkE - 1) >> 8) + 1) + x) * 4);
    bx = x << 8;
    e = (UnkEnt_023348f0 *)(base + off + 2);
    by = y << 8;
    n = *(u16 *)(base + off);
    tbl = data_ov000_023685ec;
    for (i = 0; i < n; i++, e++) {
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
                if (px > o->l && px < o->r && py > o->t && py < o->b)
                    continue;
            }
            if (e->type >= 0xf2)
                continue;
            fn = gDecorationCreateTable[e->type];
            if (fn == NULL)
                continue;
            fn(e, (bx + e->x) << 8, (by + e->y) << 8, z);
        }
    }
}
