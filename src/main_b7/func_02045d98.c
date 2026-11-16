#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSprite_02045d98;

typedef struct {
    u8 pad0[8];
    s32 y0;
    s32 y1;
} UnkSub_02045d98;

typedef struct {
    u8 pad0[0x330];
    UnkSub_02045d98 sub;
    u8 pad[0xe94 - 0x330 - sizeof(UnkSub_02045d98)];
    UnkSprite_02045d98 spr;
} UnkObj_02045d98;

void func_02032d2c(UnkSprite_02045d98 *spr);

#pragma thumb on
void func_02045d98(UnkObj_02045d98 *o) {
    UnkSub_02045d98 *s = &o->sub;
    UnkSprite_02045d98 *spr = &o->spr;
    spr->x = 0;
    spr->y = s->y0 >> 12;
    spr->flags &= ~0x100;
    func_02032d2c(spr);
    spr->x = 0;
    spr->y = s->y1 >> 12;
    spr->flags |= 0x100;
    func_02032d2c(spr);
}
#pragma thumb reset
