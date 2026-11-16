#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022d9cf8;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022d9cf8 spr[16];
} UnkStruct_022d9cf8;

void AnimatorSprite_ProcessAnimation(UnkSpr_022d9cf8 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d9cf8 *s);

#pragma thumb on
void VsMenu_Unk_ov001_022d9cf8(UnkStruct_022d9cf8 *p, s32 idx, s32 x, s32 y) {
    UnkSpr_022d9cf8 *s;

    s = &p->spr[idx + 5];
    s->x = x;
    s->y = y;
    s->unk46 = 6;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &p->spr[idx + 7];
    s->x = x + 0x48;
    s->y = y + 4;
    s->unk46 = 6;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
