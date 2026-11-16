#include <nitro/types.h>

typedef struct {
    void *unk0;
    u32 unk4;
} UnkStruct_02356434;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02356434 *work;
} UnkStruct_02356434_Task;

extern UnkStruct_02356434_Task *gTaskCurrent;

s32 Wireless_GetLinkLevel(void);
void AnimatorSprite_SetAnimation(void *spr, u16 anim);
void AnimatorSprite_ProcessAnimation(void *spr, s32 a, s32 b);
void AnimatorSprite_Draw(void *spr);

#pragma thumb on

void func_ov000_02356434(void) {
    UnkStruct_02356434 *w = gTaskCurrent->work;
    u32 anim;

    switch (Wireless_GetLinkLevel()) {
    case 0:
        anim = 1;
        break;
    case 1:
        anim = 2;
        break;
    case 2:
        anim = 3;
        break;
    case 3:
    default:
        anim = 4;
        break;
    }
    if (w->unk4 != anim) {
        w->unk4 = anim;
        AnimatorSprite_SetAnimation(w->unk0, w->unk4);
    }
    AnimatorSprite_ProcessAnimation(w->unk0, 0, 0);
    AnimatorSprite_Draw(w->unk0);
}

#pragma thumb reset
