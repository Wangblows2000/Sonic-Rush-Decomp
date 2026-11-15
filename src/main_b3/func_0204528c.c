#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_0204528c;
typedef struct { s32 x; s32 y; } UnkVec_0204528c;
typedef struct {
    u8 pad0[8];
    UnkVec_0204528c pos[6]; /* 0x8 */
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    u8 pad3E[0x5e];
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u8 padAC[4];
    u16 unkB0[5];
} UnkObj_0204528c;
typedef struct {
    u8 pad0[0x274];
    UnkObj_0204528c obj;   /* 0x274 */
    u8 pad330[0x14];
    u32 flags;             /* 0x344 */
    u8 pad348[0x54];
    UnkSpr_0204528c spr[0x33];   /* 0x39c */
    UnkSpr_0204528c spr2[0xf];   /* 0x11f4 */
} UnkWork_0204528c;

extern s16 data_02078b24[];

void func_02032d2c(UnkSpr_0204528c *s);
void func_02032480(UnkSpr_0204528c *s, s32 sx, s32 sy, s32 r);

#pragma thumb on
void func_0204528c(UnkWork_0204528c *w) {
    UnkObj_0204528c *o = &w->obj;
    UnkSpr_0204528c *a;
    UnkSpr_0204528c *b;
    UnkSpr_0204528c *spr;
    u32 i;
    s16 x;
    s16 y;

    if (w->flags & 0x10000) {
        a = &w->spr2[o->unk9C];
        b = &w->spr2[o->unk9C + 1];
        b->x = o->unkA4 >> 12;
        a->x = b->x;
        b->y = o->unkA8 >> 12;
        a->y = b->y;
        if (o->unkA0 == 0x1000 || o->unk9C == 10) {
            func_02032d2c(a);
            func_02032d2c(b);
        } else {
            func_02032480(a, o->unkA0, o->unkA0, 0);
            func_02032480(b, o->unkA0, o->unkA0, 0);
        }
    }
    if (o->unk38 != 0) {
        spr = &w->spr2[13];
        spr->x = o->pos[0].x >> 12;
        spr->y = o->pos[0].y >> 12;
        func_02032d2c(spr);
    }
    if (o->unk3A != 0) {
        x = (o->pos[1].x - 0x2a000) >> 12;
        y = o->pos[1].y >> 12;
        for (i = 0; i < 5; i++) {
            spr = &w->spr[o->unkB0[i]];
            spr->x = x + data_02078b24[i];
            spr->y = y;
            func_02032d2c(spr);
        }
        spr = &w->spr2[12];
        spr->x = x + 10;
        spr->y = y;
        func_02032d2c(spr);
        spr = &w->spr2[12];
        x += 0x28;
        spr->x = x;
        spr->y = y;
        func_02032d2c(spr);
    }
    if (o->unk3C != 0) {
        spr = &w->spr2[14];
        spr->x = o->pos[2].x >> 12;
        spr->y = o->pos[2].y >> 12;
        func_02032d2c(spr);
    }
}
#pragma thumb reset
