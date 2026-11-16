#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
} UnkSpr_02358224;

typedef struct {
    u8 pad0[0x96C];
    UnkSpr_02358224 spr;
} UnkStruct_02358224;

void func_0203344c(UnkSpr_02358224 *spr, s32 a, s32 b);
void func_02032d2c(UnkSpr_02358224 *spr);
void func_02032480(UnkSpr_02358224 *spr, s32 sx, s32 sy, s32 c);

#pragma thumb on

void func_ov000_02358224(UnkStruct_02358224 *w, s32 x, s32 scale) {
    UnkSpr_02358224 *spr;

    if (scale >= 0 && scale <= 0x1000) {
        spr = &w->spr;
        spr->unk4 = x + 0x80;
        spr->unk6 = 0xa8;
        func_0203344c(spr, 0, 0);
        if (scale == 0x1000) {
            func_02032d2c(spr);
        } else {
            func_02032480(spr, 0x1000, scale, 0);
        }
    }
}

#pragma thumb reset
