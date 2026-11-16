#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSprite_022cbecc;

typedef struct {
    u8 pad0[0x78];
    UnkSprite_022cbecc spr[10];
    u8 padx[0x390 - 0x78 - 10 * 0x48];
    u32 anim[10];
} UnkStruct_022cbecc;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void OptionsMenu_DrawItemSprite(UnkStruct_022cbecc *p, s32 idx, s16 x, s16 y, s32 sel) {
    UnkSprite_022cbecc *s;
    u32 *a;
    u32 id;
    s32 n;

    n = idx + 2;
    s = &p->spr[n];
    a = &p->anim[n];
    id = idx * 2 + 1;
    if (sel == 1) {
        id++;
    }
    if (id != *a) {
        AnimatorSprite_SetAnimation(s, id);
        *a = id;
    }
    s->x = x;
    s->y = y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
