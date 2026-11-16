#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022ca2d4;

typedef struct {
    u8 pad0[0xc0];
    UnkSpr_022ca2d4 spr;
    u8 pad[0x394 - 0xc0 - sizeof(UnkSpr_022ca2d4)];
    u32 anim;
} UnkWork_022ca2d4;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022ca2d4 *work;
} UnkStruct_022ca2d4;

void func_02033550(UnkSpr_022ca2d4 *s, u16 anim);
void func_0203344c(UnkSpr_022ca2d4 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022ca2d4 *s);

#pragma thumb on
void func_ov001_022ca2d4(UnkStruct_022ca2d4 *p, s32 x, s32 y, s32 mode) {
    UnkWork_022ca2d4 *w;
    UnkSpr_022ca2d4 *s;
    u16 anim;

    if (x < 0x100) {
        w = p->work;
        s = &w->spr;
        anim = 0;
        switch (mode) {
        case 0:
            break;
        case 1:
            anim += 1;
            break;
        case 2:
            anim += 2;
            break;
        }
        if (w->anim != anim) {
            func_02033550(s, anim);
            w->anim = anim;
        }
        x += 0xd8;
        s->x = x;
        y += 0x90;
        s->y = y;
        s->flags &= 0x60000;
        s->flags |= 0x20000;
        func_0203344c(s, 0, 0);
        func_02032d2c(s);
    }
}
#pragma thumb reset
