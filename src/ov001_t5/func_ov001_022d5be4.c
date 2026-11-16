#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
} UnkSprite_022d5be4;

typedef struct {
    u8 pad0[4];
    s32 timer;
    u8 pad8[0x32c - 8];
    UnkSprite_022d5be4 spr;
    u8 padx[0x3c4 - 0x32c - sizeof(UnkSprite_022d5be4)];
    u32 anim;
} UnkStruct_022d5be4;

void func_02033550(void *s, u16 id);
void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022d5be4(UnkStruct_022d5be4 *p) {
    UnkSprite_022d5be4 *s;
    u32 *a;

    s = &p->spr;
    a = &p->anim;
    if (*a != 0) {
        *a = 0;
        func_02033550(s, *a);
    }
    s->x = 0;
    s->y = 0;
    s->flags &= ~4;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
