#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x46 - 8];
    u16 pal;
} UnkSprite_022d9b0c;

typedef struct {
    u8 pad0[0x570];
    UnkSprite_022d9b0c spr;
} UnkStruct_022d9b0c;

void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);

#pragma thumb on
void func_ov001_022d9b0c(UnkStruct_022d9b0c *p, s16 x, s16 y) {
    UnkSprite_022d9b0c *s;

    s = &p->spr;
    s->x = x - 4;
    s->y = y - 4;
    s->pal = 4;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
}
#pragma thumb reset
