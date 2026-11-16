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

void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void VsMenu_DrawCursor(UnkStruct_022d9b0c *p, s16 x, s16 y) {
    UnkSprite_022d9b0c *s;

    s = &p->spr;
    s->x = x - 4;
    s->y = y - 4;
    s->pal = 4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
