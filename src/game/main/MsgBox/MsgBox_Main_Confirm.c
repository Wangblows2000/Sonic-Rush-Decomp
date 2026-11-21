#include <nitro/types.h>
#include <main_b3/w50.h>

extern UnkTask_W50 *gTaskCurrent;

void AnimatorSprite_ProcessAnimation(UnkSpr_W50 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_W50 *s);
void AnimatorSprite_SetAnimation(UnkSpr_W50 *s, u16 anim);
void MsgBox_BeginClose(void);

#pragma thumb on
void MsgBox_Main_Confirm(void) {
    UnkWork_W50 *w = gTaskCurrent->work;
    UnkSpr_W50 *spr;
    s32 i;
    s32 anim;

    if (w->unk20 != 3) {
        spr = &w->spr[0];
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
        AnimatorSprite_Draw(spr);
    }
    if (w->unk1C != 0x1c) {
        spr = &w->spr[1];
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
        AnimatorSprite_Draw(spr);
    }
    for (i = 0; i < w->unk8; i++) {
        spr = &w->spr[2 + i];
        anim = w->unkC[i];
        if (i == w->unk4) {
            if (w->unk8 == 1)
                anim += 1;
            else
                anim += 2;
            spr->unk2C |= 4;
        } else {
            spr->unk2C &= ~4;
        }
        if (anim != w->anim[2 + i]) {
            AnimatorSprite_SetAnimation(spr, anim);
            w->anim[2 + i] = anim;
        }
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
        AnimatorSprite_Draw(spr);
    }
    if (w->unk24 == 1) {
        spr = &w->spr[4];
        if (w->anim[4] != 0) {
            AnimatorSprite_SetAnimation(spr, 0);
            w->anim[2 + i] = 0;
        }
        if (w->unk8 == 1) {
            spr->x = 0x60;
            spr->y = 0x6c;
        } else if (w->unk4 == 0) {
            spr->x = 0x3c;
            spr->y = 0x68;
        } else {
            spr->x = 0x7c;
            spr->y = 0x68;
        }
        spr->unk2C &= ~4;
        if (w->unk8 > 1) {
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
        }
    }
    w->unk0++;
    if (w->unk0 >= 0x20)
        MsgBox_BeginClose();
}
#pragma thumb reset
