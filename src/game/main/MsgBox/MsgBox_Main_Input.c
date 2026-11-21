#include <nitro/types.h>
#include <main_b3/w50.h>

extern UnkTask_W50 *gTaskCurrent;

void AnimatorSprite_ProcessAnimation(UnkSpr_W50 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_W50 *s);
void AnimatorSprite_SetAnimation(UnkSpr_W50 *s, u16 anim);
BOOL MsgBox_IsCenterButtonTouched(void);
BOOL MsgBox_IsLeftButtonTouched(void);
BOOL MsgBox_IsRightButtonTouched(void);
BOOL MsgBox_IsTouched(void);
void SysSound_PlayDecideSE(void);
void SysSound_PlayCancelSE(void);
void SysSound_PlaySE(s32 a);
void SysSound_PlayCursorSE(void);
void MsgBox_BeginClose(void);
void MsgBox_BeginConfirm(void);

#pragma thumb on
void MsgBox_Main_Input(void) {
    UnkWork_W50 *w = gTaskCurrent->work;
    UnkSpr_W50 *spr;
    s32 i;
    s32 anim;
    BOOL done;
    BOOL a;
    BOOL b;
    u16 pad;

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
        if (i == w->unk4 && w->unk8 == 2)
            anim++;
        if (anim != w->anim[2 + i]) {
            AnimatorSprite_SetAnimation(spr, anim);
            w->anim[2 + i] = anim;
        }
        spr->unk2C &= ~4;
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
        AnimatorSprite_Draw(spr);
    }
    if (w->unk24 == 1) {
        spr = &w->spr[4];
        anim = 0;
        if (w->unk0 < 6) {
            anim++;
            spr->unk2C |= 4;
        } else {
            spr->unk2C &= ~4;
        }
        if (anim != w->anim[4]) {
            AnimatorSprite_SetAnimation(spr, anim);
            w->anim[2 + i] = anim;
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
        if (w->unk8 > 1) {
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
        }
    }
    done = FALSE;
    a = FALSE;
    b = FALSE;
    pad = gPad.trg;
    if (pad & 1)
        a = TRUE;
    if (pad & 2)
        b = TRUE;
    switch (w->unk24) {
    case 0:
        if (MsgBox_IsCenterButtonTouched() || a) {
            done = TRUE;
            SysSound_PlayDecideSE();
        }
        break;
    case 1:
        if (MsgBox_IsLeftButtonTouched()) {
            w->unk4 = 0;
            done = TRUE;
            if (w->unk14 & 1)
                SysSound_PlaySE(0x12);
            else
                SysSound_PlayDecideSE();
        } else if (MsgBox_IsRightButtonTouched()) {
            done = TRUE;
            w->unk4 = 1;
            SysSound_PlayDecideSE();
        } else if (a) {
            done = TRUE;
            if (w->unk4 == 0 && (w->unk14 & 1))
                SysSound_PlaySE(0x12);
            else
                SysSound_PlayDecideSE();
        } else if (b) {
            done = TRUE;
            w->unk4 = 1;
            SysSound_PlayCancelSE();
        }
        break;
    case 2:
        if (MsgBox_IsCenterButtonTouched() || a) {
            done = TRUE;
            SysSound_PlayDecideSE();
        }
        break;
    case 3:
        if (MsgBox_IsTouched() || a) {
            done = TRUE;
            SysSound_PlayDecideSE();
        }
        break;
    case 4:
    case 5:
        break;
    }
    if (!done) {
        pad = gPad.trg;
        if (pad & 0x20) {
            if (w->unk4 > 0) {
                w->unk4--;
                w->unk0 = 0;
                SysSound_PlayCursorSE();
            }
        } else if (pad & 0x10) {
            if (w->unk4 < w->unk8 - 1) {
                w->unk4++;
                w->unk0 = 0;
                SysSound_PlayCursorSE();
            }
        }
    }
    w->unk0++;
    if (done) {
        if (b || w->unk24 == 3)
            MsgBox_BeginClose();
        else
            MsgBox_BeginConfirm();
    } else if (w->unk48) {
        MsgBox_BeginClose();
    }
}
#pragma thumb reset
