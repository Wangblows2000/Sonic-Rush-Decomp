#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022cbdf0;

typedef struct {
    u8 pad0[0x78];
    UnkSprite_022cbdf0 spr;
    u8 padx[0x390 - 0x78 - sizeof(UnkSprite_022cbdf0)];
    u32 anim;
} UnkStruct_022cbdf0;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void OptionsMenu_DrawListBackButton(UnkStruct_022cbdf0 *p, s16 x, s16 y, s32 sel) {
    UnkSprite_022cbdf0 *s;
    u32 *a;
    u32 id;

    s = &p->spr;
    a = &p->anim;
    id = 0;
    if (sel == 1) {
        id++;
    }
    if (id != *a) {
        AnimatorSprite_SetAnimation(s, id);
        *a = id;
    }
    s->x = x;
    s->y = y;
    s->flags &= 0x60000;
    s->flags |= 0x60000;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
