#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct {
    s32 unk0;
    u8 pad4[4];
    u32 unk8;
    u8 padC[4];
    UnkSpr_W50 spr[4]; /* 0x10 */
} UnkWork_02063030;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02063030;
typedef struct { u8 pad0[0xc]; s16 unkC; } UnkMosaic_02063030;
extern UnkBlend_02063030 data_0208926c;
extern UnkBlend_02063030 data_02087d44;
extern UnkMosaic_02063030 data_0208922c;
extern UnkMosaic_02063030 data_02087d04;

s32 func_020428d4(void);
void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_W50 *s);
void func_02033550(UnkSpr_W50 *s, u16 anim);

#pragma thumb on
void func_02063030(UnkWork_02063030 *w) {
    UnkSpr_W50 *spr;
    u16 anim;

    if (w->unk0 != 0) {
        if (w->unk8 < 0x20) {
            w->unk8++;
            return;
        }
        if (w->unk8 == 0x20) {
            *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1800;
            *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x800;
        }
        if (w->unk8 <= 0x30) {
            if (data_0208926c.unk8 >= 0) {
                data_0208926c.unk8 = 0x10 - (w->unk8 - 0x20);
                data_02087d44.unk8 = 0x10 - (w->unk8 - 0x20);
            } else {
                data_0208926c.unk8 = -(0x10 - (w->unk8 - 0x20));
                data_02087d44.unk8 = -(0x10 - (w->unk8 - 0x20));
            }
        } else {
            data_0208926c.unk8 = 0;
            data_02087d44.unk8 = 0;
        }
        data_0208922c.unkC = w->unk8 << 1;
        data_02087d04.unkC = w->unk8 << 1;
        spr = &w->spr[3];
        anim = func_020428d4() + 1;
        spr->x = 0xe0;
        spr->y = 8;
        func_02033550(spr, anim);
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        spr = &w->spr[0];
        spr->x = 0x50;
        spr->y = 8;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        spr = &w->spr[1];
        spr->x = 0x98;
        spr->y = 0xc;
        spr->unk2C |= 4;
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        spr = &w->spr[2];
        func_0203344c(spr, 0, 0);
        func_02032d2c(spr);
        w->unk8++;
    }
}
#pragma thumb reset
