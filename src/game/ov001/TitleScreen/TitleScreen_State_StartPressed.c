#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022d49a0;

typedef struct {
    s32 timer;
    UnkSpr_022d49a0 sprB;
    UnkSpr_022d49a0 sprA;
    u32 animB;
    u32 animA;
} UnkWork_022d49a0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d49a0 *work;
} UnkTask_022d49a0;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d49a0;

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022c46c0;

extern UnkTask_022d49a0 *gTaskCurrent;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c46c0 data_022c46c0;
extern UnkBright_022d49a0 data_0208926c;
extern UnkBright_022d49a0 data_02087d44;

void AnimatorSprite_SetAnimation(UnkSpr_022d49a0 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d49a0 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d49a0 *s);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void TitleScreen_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void TitleScreen_GoToUnlockNotice(void);
void TitleScreen_GoToTitleMenu(void);

#pragma thumb on
void TitleScreen_State_StartPressed(void) {
    UnkWork_022d49a0 *w;
    BOOL flag;
    UnkSpr_022d49a0 *s;
    u32 *a;

    w = gTaskCurrent->work;
    s = &w->sprB;
    a = &w->animB;
    if (*a != 1) {
        *a = 1;
        AnimatorSprite_SetAnimation(s, *a);
    }
    s->flags |= 4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &w->sprA;
    a = &w->animA;
    if (*a != 1) {
        *a = 1;
        AnimatorSprite_SetAnimation(s, *a);
    }
    s->flags |= 4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    if ((gSaveData.unkc >= 0x1a || data_022c46c0.unk24 >= 0x1c) && (!(gSaveData.flags & 0x100) || !(gSaveData.flags & 0x200))) {
        flag = FALSE;
    } else {
        flag = TRUE;
    }
    if (w->timer == 0x10) {
        if (flag) {
            TitleScreen_CreateFadeTask(0, 0x10, 0x10, 0);
        } else {
            TitleScreen_CreateFadeTask(0, -0x10, 0x10, 0);
        }
    }
    w->timer++;
    if (w->timer >= 0x20) {
        w->timer = 0;
        if (flag) {
            data_0208926c.bright = 0x10;
            data_02087d44.bright = 0x10;
        } else {
            data_0208926c.bright = -0x10;
            data_02087d44.bright = -0x10;
        }
        GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.bright);
        Task_DestroyAll();
        if (!flag) {
            TitleScreen_GoToUnlockNotice();
            return;
        }
        TitleScreen_GoToTitleMenu();
    }
}
#pragma thumb reset
