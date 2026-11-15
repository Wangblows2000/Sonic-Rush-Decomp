#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct { s32 x; s32 y; } UnkVec_02045ec0;
typedef struct {
    u8 pad0[8];
    UnkVec_02045ec0 pos[6]; /* 0x8 */
    s16 unk38[4];           /* 0x38 */
    s16 unk40;
    u8 pad42[0xe];
    u16 *unk50;
    s16 digits[6][6];       /* 0x54 */
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
} UnkObj_02045ec0;

typedef struct {
    u8 pad0[0x274];
    UnkObj_02045ec0 obj;   /* 0x274 */
    u8 pad320[0x24];
    u32 flags;             /* 0x344 */
    u8 pad348[0x54];
    UnkSpr_W50 spr[0x30];  /* 0x39c */
} UnkWork_02045ec0;

extern u16 data_02078b3c[];

void func_02032d2c(UnkSpr_W50 *s);
void func_02032480(UnkSpr_W50 *s, s32 sx, s32 sy, s32 r);

#pragma thumb on
void func_02045ec0(UnkWork_02045ec0 *w) {
    UnkObj_02045ec0 *o = &w->obj;
    UnkSpr_W50 *spr;
    u32 i;
    u32 j;
    u16 idx;
    s16 d;
    s16 bx;
    s32 by;
    s16 yy;

    if (w->flags & 0x10000) {
        spr = &w->spr[o->unk9C];
        spr->x = o->unkA4 >> 12;
        spr->y = o->unkA8 >> 12;
        if (o->unkA0 == 0x1000)
            func_02032d2c(spr);
        else
            func_02032480(spr, o->unkA0, o->unkA0, 0);
    }
    for (i = 0; i < 4; i++) {
        if (o->unk38[i] != 0) {
            idx = o->unk50[i];
            if (idx != 0xffff) {
                spr = &w->spr[idx];
                spr->x = o->pos[i].x >> 12;
                spr->y = o->pos[i].y >> 12;
                func_02032d2c(spr);
                bx = spr->x + 10;
                by = spr->y;
                for (j = 0; j < 6; j++) {
                    d = o->digits[i][j];
                    if (d != -1) {
                        spr = &w->spr[d + 0x13];
                        spr->x = bx + ((j * 0xa000) >> 12);
                        spr->y = by;
                        func_02032d2c(spr);
                    }
                }
            }
        }
    }
    if (o->unk40 != 0) {
        spr = &w->spr[data_02078b3c[4]];
        spr->x = (o->pos[4].x + 0x40000) >> 12;
        spr->y = o->pos[4].y >> 12;
        func_02032d2c(spr);
    }
    if (w->flags & 0x4000) {
        spr = &w->spr[33];
        spr->x = 0x56;
        spr->y = o->pos[5].y >> 12;
        func_02032d2c(spr);
        yy = o->pos[5].y >> 12;
        for (j = 0; j < 6; j++) {
            d = o->digits[4][j];
            if (d != -1) {
                spr = &w->spr[d + 0x28];
                spr->x = ((j * 0xa000) >> 12) + 0x60;
                spr->y = yy;
                func_02032d2c(spr);
            }
        }
    }
}
#pragma thumb reset
