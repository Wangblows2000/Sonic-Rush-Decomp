#include <nitro/types.h>

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 state;
    s32 val;
    s32 timer2;
} UnkWork_022d5e78;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d5e78 *work;
} UnkTask_022d5e78;

extern UnkTask_022d5e78 *gTaskCurrent;

void TitleDemo_Unk_ov001_022d5be4(UnkWork_022d5e78 *w);
s32 TitleDemo_Interpolate(s32 from, s32 to, s32 n, s32 i, s32 ease);
void TitleDemo_SetSubBrightness(UnkWork_022d5e78 *w, s32 v);
void TitleDemo_SetMainBrightness(UnkWork_022d5e78 *w, s32 v);
BOOL TitleDemo_CheckSkipInput(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void TitleDemo_SkipToTitleScreen(void);
void TitleDemo_Action_Scene5(s32 a);
void TitleDemo_SetSubBgAffine(UnkWork_022d5e78 *w, s32 scale, u16 angle);
void TitleDemo_PlaySeq3(UnkWork_022d5e78 *w);
void TitleDemo_Unk_ov001_022d5b44(UnkWork_022d5e78 *w, s32 a);
void TitleDemo_Unk_ov001_022d5b94(UnkWork_022d5e78 *w, s32 a);
void TitleDemo_Unk_ov001_022d5a18(UnkWork_022d5e78 *w);
void TitleDemo_PlaySeq4(UnkWork_022d5e78 *w);
void TitleDemo_SetOrthoProjection(UnkWork_022d5e78 *w);
void TitleDemo_SetupCamera(UnkWork_022d5e78 *w);
void TitleDemo_DrawSonicModel(UnkWork_022d5e78 *w, s32 a, s32 b);
void TitleDemo_DrawBlazeModel(UnkWork_022d5e78 *w, s32 a, s32 b);

#pragma thumb on
void TitleDemo_State_Scene4(void) {
    UnkWork_022d5e78 *w;
    s32 t;
    s32 v;

    w = gTaskCurrent->work;
    TitleDemo_Unk_ov001_022d5be4(w);
    TitleDemo_Unk_ov001_022d5b44(w, 0);
    TitleDemo_Unk_ov001_022d5b94(w, 0);
    t = w->timer;
    if (t >= 0) {
        v = TitleDemo_Interpolate(0x800, 0x1000, 0x20, t, 0x2000);
        TitleDemo_SetSubBgAffine(w, v, TitleDemo_Interpolate(0x28000, 0, 0x20, t, 0x2000));
        if (t == 0) {
            TitleDemo_PlaySeq3(w);
        }
    }
    if (w->timer >= 0) {
        v = TitleDemo_Interpolate(0x10, 0, 0x20, w->timer, 0x1000);
        TitleDemo_SetSubBrightness(w, 0);
        TitleDemo_SetMainBrightness(w, v);
    } else {
        TitleDemo_SetSubBrightness(w, 0);
        TitleDemo_SetMainBrightness(w, 0x10);
    }
    if (w->timer >= 0) {
        v = TitleDemo_Interpolate(0x80, 0x28, 0x20, w->timer, 0x2000);
        TitleDemo_SetOrthoProjection(w);
        TitleDemo_SetupCamera(w);
        TitleDemo_DrawSonicModel(w, v, -0x40);
        TitleDemo_DrawBlazeModel(w, -v, -0x40);
    }
    if (w->state == 0 && TitleDemo_CheckSkipInput()) {
        w->state = 1;
        w->val = 0;
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
        TitleDemo_SetMainBrightness(w, 0);
        TitleDemo_Action_Scene5(0);
    }
}
#pragma thumb reset
