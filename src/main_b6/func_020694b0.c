#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 angle;
    u8 pad2[2];
    u8 b4;
    u8 b5;
    u8 pad6[10];
    s32 x10;
    s32 x14;
    s32 x18;
    s32 x1c;
    s32 x20;
    s32 x24;
    s16 h28;
    s16 h2a;
    s16 h2c;
    s16 h2e;
    s16 h30;
    s16 h32;
    s16 h34;
    s16 h36;
    s16 h38;
    s16 h3a;
    s16 h3c;
    s16 h3e;
    s16 h40;
    s16 h42;
    u8 pad44[0x10];
    u32 flags54;
    u32 flags58;
    u16 h5c;
    u8 pad5e[4];
    u16 h62;
} UnkObj_020694b0;

typedef struct {
    s16 x;
    s16 y;
} UnkVec_020694b0;

extern s16 data_0207cf7c;
extern s16 data_0206e470[];
extern UnkVec_020694b0 data_022c4ef8;

void func_020693f0(s16 *x, s16 *y, u8 a);
s16 func_02068df8(s32 v, s32 a, s32 m);

#define SIN_IDX(i) data_0206e470[(((i) & 0xffff) >> 4) << 1]
#define COS_IDX(i) data_0206e470[((((i) & 0xffff) >> 4) << 1) + 1]

void func_020694b0(UnkObj_020694b0 *o) {
    s32 dy = 0;
    s32 dx = 0;
    UnkVec_020694b0 v;

    v.x = o->h36;
    v.y = o->h38;
    o->x1c = o->x10;
    o->x20 = o->x14;
    o->x24 = o->x18;
    if (o->flags58 & 0x10000000) {
        o->h36 = 0;
        o->h38 = 0;
        o->h3a = 0;
    }
    if ((v.x != 0 || v.y != 0) && o->b5 != 0)
        func_020693f0(&v.x, &v.y, o->b5);
    if (o->h62 & 0xf) {
        o->h28 = (v.x * data_0207cf7c) >> 8;
        o->h2a = (v.y * data_0207cf7c) >> 8;
        o->h2c = (o->h3a * data_0207cf7c) >> 8;
    } else {
        if ((o->flags58 & 1) == 0) {
            if ((o->flags58 & 0x80) && (o->flags58 & 1) == 0)
                o->h30 += (o->h40 * data_0207cf7c) >> 8;
            if ((o->flags58 & 0x80) && o->h30 > o->h42)
                o->h30 = o->h42;
        }
        if (o->flags58 & 0x40) {
            if ((o->flags58 & 0x20000) && (o->h34 != 0 || (o->flags58 & 0x40000) == 0)
                && (u8)(o->angle + o->b4) >= o->b4 * 2) {
                if (o->angle != 0)
                    o->h34 = func_02068df8(o->h34, (o->h3c * SIN_IDX((u16)(o->angle << 8))) >> 12, o->h3e);
                else
                    o->h34 = func_02068df8(o->h34, (-o->h3c * SIN_IDX((u16)(o->angle << 8))) >> 12, o->h3e);
            }
            if ((o->flags58 & 0x8000) == 0) {
                dy = (s16)((o->h34 * COS_IDX((u16)(o->angle << 8))) >> 12);
                dx = (s16)((o->h34 * SIN_IDX((u16)(o->angle << 8))) >> 12);
            }
        }
        if (o->flags58 & 0x8000000) {
            o->h28 = ((o->h2e + dy + v.x) * data_0207cf7c) >> 8;
            o->h2a = ((o->h30 + dx + v.y) * data_0207cf7c) >> 8;
        } else {
            o->h28 = ((o->h2e + dy + v.x + data_022c4ef8.x) * data_0207cf7c) >> 8;
            o->h2a = ((o->h30 + dx + v.y + data_022c4ef8.y) * data_0207cf7c) >> 8;
        }
        o->h2c = ((o->h3a + o->h32) * data_0207cf7c) >> 8;
        func_020693f0(&o->h28, &o->h2a, o->b5);
    }
    o->h36 = 0;
    o->h38 = 0;
    o->h3a = 0;
    o->x10 += o->h28;
    o->x14 += o->h2a;
    o->x18 += o->h2c;
}
