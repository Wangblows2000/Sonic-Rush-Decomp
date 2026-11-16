#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
} UnkSpr_023580bc;

typedef struct {
    u8 pad0[0x5C4];
    UnkSpr_023580bc spr;
} UnkStruct_023580bc;

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_023580bc;

extern UnkOfs_023580bc data_02087d04[4];

void func_0203344c(UnkSpr_023580bc *spr, s32 a, s32 b);
void func_02032d2c(UnkSpr_023580bc *spr);

#pragma thumb on

void func_ov000_023580bc(UnkStruct_023580bc *w, BOOL flag) {
    UnkSpr_023580bc *spr = &w->spr;

    spr->unk4 = 0;
    spr->unk6 = 0x10;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);
    if (flag) {
        data_02087d04[2].y = -8;
    } else {
        data_02087d04[2].y = 0x20;
    }
}

#pragma thumb reset
