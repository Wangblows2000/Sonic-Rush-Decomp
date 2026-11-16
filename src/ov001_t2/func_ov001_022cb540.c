#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022cb540;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padc[0x2b8 - 0xc];
    UnkSpr_022cb540 spr;
    u8 pad2c0[0x3b0 - 0x2c0];
    u32 anim;
} UnkWork_022cb540;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb540 *work;
} UnkStruct_022cb540;

void func_02033550(UnkSpr_022cb540 *s, u16 anim);
void func_0203344c(UnkSpr_022cb540 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022cb540 *s);

#pragma thumb on
void func_ov001_022cb540(UnkStruct_022cb540 *p, s32 x, s32 y, BOOL alt) {
    UnkWork_022cb540 *w = p->work;
    UnkSpr_022cb540 *s = &w->spr;
    u16 anim;

    switch (w->unk8) {
    case 0:
        anim = 0xd;
        break;
    case 1:
        anim = 0xf;
        break;
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
