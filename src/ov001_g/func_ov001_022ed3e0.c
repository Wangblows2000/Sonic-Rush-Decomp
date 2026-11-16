#include <nitro/types.h>

typedef struct UnkSpr_022ed3e0 {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_022ed3e0;

typedef struct UnkStruct_022ed3e0 {
    u8 pad0[0x4e4];
    UnkSpr_022ed3e0 spr[5];
    u8 pad64c[0x68c - 0x64c];
    u32 anim[5];
} UnkStruct_022ed3e0;

void func_02033550(UnkSpr_022ed3e0 *s, u16 anim);
void func_0203344c(UnkSpr_022ed3e0 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ed3e0 *s);

#pragma thumb on
void func_ov001_022ed3e0(UnkStruct_022ed3e0 *p) {
    UnkSpr_022ed3e0 *s;
    u32 *anim;

    s = &p->spr[0];
    anim = &p->anim[0];
    if (*anim != 1) {
        *anim = 1;
        func_02033550(s, *anim);
    }
    func_0203344c(s, 0, 0);
    func_02032d2c(s);

    s = &p->spr[1];
    anim = &p->anim[1];
    if (*anim != 2) {
        *anim = 2;
        func_02033550(s, *anim);
    }
    func_0203344c(s, 0, 0);
    func_02032d2c(s);

    s = &p->spr[2];
    anim = &p->anim[2];
    if (*anim != 3) {
        *anim = 3;
        func_02033550(s, *anim);
    }
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
