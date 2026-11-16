#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022cb4d8;

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    u8 pad10[0x300 - 0x10];
    UnkSpr_022cb4d8 spr;
    u8 pad308[0x3b4 - 0x308];
    u32 anim;
} UnkWork_022cb4d8;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb4d8 *work;
} UnkStruct_022cb4d8;

void func_02033550(UnkSpr_022cb4d8 *s, u16 anim);
void func_0203344c(UnkSpr_022cb4d8 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022cb4d8 *s);

#pragma thumb on
void func_ov001_022cb4d8(UnkStruct_022cb4d8 *p, s32 x, s32 y, BOOL alt) {
    UnkWork_022cb4d8 *w = p->work;
    UnkSpr_022cb4d8 *s = &w->spr;
    u16 anim;

    if (w->unkc != 0) {
        anim = 0x11;
    } else {
        anim = 0x13;
    }
    if (alt) {
        anim++;
    }
    if (w->anim != anim) {
        func_02033550(s, anim);
        w->anim = anim;
    }
    s->x = x - 0x44;
    s->y = y;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
