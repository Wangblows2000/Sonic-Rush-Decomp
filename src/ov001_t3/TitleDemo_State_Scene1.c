#include <nitro/types.h>

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 state;
    s32 val;
    s32 timer2;
} UnkWork_022d6378;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d6378 *work;
} UnkTask_022d6378;

extern UnkTask_022d6378 *gTaskCurrent;

void TitleDemo_Unk_ov001_022d5be4(UnkWork_022d6378 *w);
s32 TitleDemo_Interpolate(s32 from, s32 to, s32 n, s32 i, s32 ease);
void TitleDemo_SetSubBrightness(UnkWork_022d6378 *w, s32 v);
void TitleDemo_SetMainBrightness(UnkWork_022d6378 *w, s32 v);
BOOL TitleDemo_CheckSkipInput(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void TitleDemo_SkipToTitleScreen(void);
void TitleDemo_Action_Scene2(s32 a);

#pragma thumb on
void TitleDemo_State_Scene1(void) {
    UnkWork_022d6378 *w;

    w = gTaskCurrent->work;
    TitleDemo_Unk_ov001_022d5be4(w);
    if (w->timer >= 0) {
        TitleDemo_SetSubBrightness(w, TitleDemo_Interpolate(0x10, 0, 0x20, w->timer, 0x1000));
        TitleDemo_SetMainBrightness(w, 0x10);
    } else {
        TitleDemo_SetSubBrightness(w, 0x10);
        TitleDemo_SetMainBrightness(w, 0x10);
    }
    if (w->state == 0 && w->timer > 0 && TitleDemo_CheckSkipInput()) {
        w->state = 1;
        w->val = 0;
        w->timer2 = 0;
    }
    if (w->state == 1) {
        if (w->timer2 >= 0) {
            w->val = TitleDemo_Interpolate(0, 0x10, 0x20, w->timer2, 0x1000);
        }
        w->timer2++;
        if (w->timer2 >= 0x20) {
            TitleDemo_SetSubBrightness(w, 0x10);
            TitleDemo_SetMainBrightness(w, 0x10);
            GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
            GXx_SetMasterBrightness_((vu16 *)0x400106c, 0x10);
            Task_DestroyAll();
            TitleDemo_SkipToTitleScreen();
            return;
        }
    }
    w->timer++;
    if (w->timer >= 0x20) {
        TitleDemo_SetSubBrightness(w, 0);
        TitleDemo_SetMainBrightness(w, 0x10);
        TitleDemo_Action_Scene2(0);
    }
}
#pragma thumb reset
