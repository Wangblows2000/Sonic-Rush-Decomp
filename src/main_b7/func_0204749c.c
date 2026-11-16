#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s32 work[20];
} UnkSub_0204749c;

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSprite_0204749c;

typedef struct {
    u8 pad0[0x200];
    UnkSub_0204749c sub;
    u8 pad[0x39c - 0x200 - sizeof(UnkSub_0204749c)];
    UnkSprite_0204749c sprites[20];
} UnkObj_0204749c;

void AnimatorSprite_Draw(UnkSprite_0204749c *spr);

#pragma thumb on
void func_0204749c(UnkObj_0204749c *o) {
    UnkSub_0204749c *s = &o->sub;
    u32 i;
    for (i = 0; i < s->count; i++) {
        s16 x = (s->unkc + s->work[i]) >> 12;
        s16 y = s->unk10 >> 12;
        u8 t = s->table[i];
        if (t != 0x13) {
            UnkSprite_0204749c *spr = &o->sprites[t];
            spr->x = x;
            spr->y = y;
            AnimatorSprite_Draw(spr);
        }
    }
}
#pragma thumb reset
