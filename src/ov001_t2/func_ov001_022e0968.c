#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3c];
    u16 unk44;
} UnkSpr_022e0968;

typedef struct {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0xd8 - 8];
    s32 unkd8;
    u8 paddc[0x6c0 - 0xdc];
    UnkSpr_022e0968 spr;
    u8 pad[0xe40 - 0x6c0 - sizeof(UnkSpr_022e0968)];
    s32 anim;
} UnkStruct_022e0968;

extern s32 data_ov001_0231c3f0;

void func_02033550(UnkSpr_022e0968 *s, u16 anim);
void func_0203344c(UnkSpr_022e0968 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022e0968 *s);

#pragma thumb on
void func_ov001_022e0968(UnkStruct_022e0968 *p) {
    UnkSpr_022e0968 *s = &p->spr;
    s32 anim = 0x18;

    if (p->unk4 < 6 && p->unkd8 == 0) {
        anim++;
    }
    if (anim != p->anim) {
        func_02033550(s, anim);
        p->anim = anim;
    }
    s->unk44 = 0;
    s->x = 0x10;
    s->y = data_ov001_0231c3f0 * 32 + 0x38;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
