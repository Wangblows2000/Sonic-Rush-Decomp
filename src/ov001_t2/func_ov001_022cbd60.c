#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
} UnkSpr_022cbd60;

typedef struct {
    u8 pad0[0x18c8];
    UnkSpr_022cbd60 spr;
    u8 pad[0x1ba0 - 0x18c8 - sizeof(UnkSpr_022cbd60)];
    u32 anim;
} UnkStruct_022cbd60;

s32 func_020428d4(UnkStruct_022cbd60 *p);
void func_02033550(UnkSpr_022cbd60 *s, u16 anim);
void func_0203344c(UnkSpr_022cbd60 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022cbd60 *s);

#pragma thumb on
void func_ov001_022cbd60(UnkStruct_022cbd60 *p, s32 x, s32 y) {
    UnkSpr_022cbd60 *s = &p->spr;
    u32 *cur = &p->anim;
    u32 anim;

    switch (func_020428d4(p)) {
    case 0:
        anim = 1;
        break;
    case 1:
        anim = 2;
        break;
    case 2:
        anim = 3;
        break;
    case 3:
        anim = 4;
        break;
    }
    if (anim != *cur) {
        func_02033550(s, anim);
        *cur = anim;
    }
    s->x = x;
    s->y = y;
    s->flags &= 0x60000;
    s->flags |= 0x20000;
    s->unk44 = 1;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
