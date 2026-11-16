#include <nitro/types.h>

typedef struct {
    s32 l;
    s32 t;
    s32 r;
    s32 b;
} UnkStruct_02334eb4_R;

typedef struct {
    u8 pad0[4];
    s32 x;
    s32 y;
    u8 padC[0x1E - 0xC];
    s16 w;
    s16 h;
    u8 pad22[0x64 - 0x22];
} UnkStruct_02334eb4_O;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
} UnkStruct_02334eb4_M;

typedef void (*UnkFunc_02334eb4)(void *a, u16 x, u16 y, UnkStruct_02334eb4_R *r, UnkStruct_02334eb4_R *o, u32 f);

UnkStruct_02334eb4_O *func_ov000_0233b62c(void);
extern UnkStruct_02334eb4_M data_ov000_0236abd8;
extern UnkFunc_02334eb4 data_ov000_0236aa98;
extern u16 *data_ov000_02364d54[];
extern UnkFunc_02334eb4 data_ov000_023687d4[];

void func_ov000_02334eb4(void *a, s32 idx, u32 flags) {
    u16 ww = data_ov000_0236abd8.unkE;
    u16 hh = data_ov000_0236abd8.unk10;
    UnkStruct_02334eb4_O *o = func_ov000_0233b62c();
    UnkStruct_02334eb4_R rc;
    UnkStruct_02334eb4_R base;
    u8 i;
    s32 maxx;
    s32 maxy;
    s32 minx;
    s32 miny;
    u32 f;
    u16 r;
    u16 *rp;
    u16 xx;
    u16 yy;

    base.l = o->x >> 8;
    base.t = o->y >> 8;
    base.r = (o->x + (o->w << 8)) >> 8;
    base.b = (o->y + o->h * 0x1d0) >> 8;
    maxx = ww - 1;
    maxy = hh - 1;
    minx = 0;
    miny = 0;
    for (i = 0; i < 3; i++) {
        rp = data_ov000_02364d54[i];
        r = *rp;
        data_ov000_0236aa98 = data_ov000_023687d4[i];
        f = flags;
        rc.l = (o->x >> 8) - r;
        rc.t = (o->y >> 8) - r;
        rc.r = r + ((o->x + (o->w << 8)) >> 8);
        rc.b = r + ((o->y + o->h * 0x1d0) >> 8);
        if (rc.l <= 0) {
            rc.l = minx;
            f |= 0x10;
        }
        if (rc.t <= 0) {
            rc.t = miny;
            f |= 0x20;
        }
        if (rc.r >= maxx) {
            rc.r = maxx;
            f |= 0x40;
        }
        if (rc.b >= maxy) {
            rc.b = maxy;
            f |= 0x80;
        }
        for (yy = rc.t >> 8; yy <= rc.b >> 8; yy++) {
            for (xx = rc.l >> 8; xx <= rc.r >> 8; xx++) {
                data_ov000_0236aa98(a, xx, yy, &rc, &base, f);
            }
        }
    }
}
