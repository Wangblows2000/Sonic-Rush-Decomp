#include <nitro/types.h>

typedef struct {
    u8 pad0[0x44];
    u16 unk44;
    u8 pad46[2];
} UnkSpr_022e08e0;

typedef struct {
    u8 pad0[0x558];
    UnkSpr_022e08e0 spr0;
    UnkSpr_022e08e0 spr1;
    u8 pad5e8[0xe2c - 0x5e8];
    s32 anim0;
    s32 anim1;
} UnkStruct_022e08e0;

extern s32 data_ov001_0231c3ec;

void func_02033550(UnkSpr_022e08e0 *s, u16 anim);
void func_0203344c(UnkSpr_022e08e0 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022e08e0 *s);

#pragma thumb on
void func_ov001_022e08e0(UnkStruct_022e08e0 *p) {
    UnkSpr_022e08e0 *s = &p->spr0;
    s32 anim;

    if (p->anim0 != 7) {
        func_02033550(s, 7);
        p->anim0 = 7;
    }
    s->unk44 = 0xd;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
    s = &p->spr1;
    anim = data_ov001_0231c3ec + 8;
    if (anim != p->anim1) {
        func_02033550(s, anim);
        p->anim1 = anim;
    }
    s->unk44 = 0xd;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
