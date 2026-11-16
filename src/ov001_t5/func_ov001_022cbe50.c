#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSprite_022cbe50;

typedef struct {
    u8 pad0[0x3bc];
    UnkSprite_022cbe50 spr;
    u32 anim;
} UnkStruct_022cbe50;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022cbe50(UnkStruct_022cbe50 *p, s16 x, s16 y) {
    UnkSprite_022cbe50 *s;
    u32 *a;

    s = &p->spr;
    a = &p->anim;
    if (*a != 0) {
        func_02033550(s, 0);
        *a = 0;
    }
    s->x = x;
    s->y = y;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
