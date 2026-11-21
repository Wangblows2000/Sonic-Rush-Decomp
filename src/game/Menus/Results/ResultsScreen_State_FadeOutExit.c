#include <game/Stage/stageobj_alt.h>

typedef struct {
    u32 unk0;
    u8 pad4[4];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x20 - 0x10];
    s32 unk20;
    u8 pad24[0xC14 - 0x24];
    u8 unkC14[4];
} UnkStruct_0235e340;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235e340_Bright;

extern UnkStruct_Task *gTaskCurrent;
extern UnkStruct_0235e340_Bright data_0208926c;
extern UnkStruct_0235e340_Bright data_02087d44;

void ResultsScreen_DrawHeader(UnkStruct_0235e340 *w);
void ResultsScreen_DrawSprite2(UnkStruct_0235e340 *w);
void ResultsScreen_DrawSprite1Pair(UnkStruct_0235e340 *w);
void ResultsScreen_DrawModels(UnkStruct_0235e340 *w);
void ResultsScreen_DrawWinLoseSprites(UnkStruct_0235e340 *w, s32 a, s32 b);
s32 ResultsScreen_EaseValue(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void ResultsScreen_SaveVsResult(s32 a, void *b);
void SaveData_CommitOrShowError(s32 a);
void Task_DestroyAll(void);
BOOL ResultsScreen_Unk_ov000_0235cbd8(void);
void ResultsScreen_Exit(s32 a);
void WirelessSession_Stop(void);
void ResultsScreen_ShowLinkLostMessage(void);

#pragma thumb on

void ResultsScreen_State_FadeOutExit(void) {
    UnkStruct_0235e340 *w = gTaskCurrent->work;
    s32 t;
    s32 target;
    s32 next;
    s16 b;

    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    ResultsScreen_DrawModels(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    t = w->unk0;
    if (w->unkC != 0)
        target = -0x10;
    else
        target = 0x10;
    if (t >= 0) {
        b = ResultsScreen_EaseValue(0, target, 0x10, t, 0x1000);
        data_0208926c.unk8 = b;
        data_02087d44.unk8 = b;
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        next = w->unk8;
        b = target;
        data_0208926c.unk8 = b;
        data_02087d44.unk8 = b;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, b);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, b);
        ResultsScreen_SaveVsResult(w->unk20, w->unkC14);
        SaveData_CommitOrShowError(0);
        Task_DestroyAll();
        if (ResultsScreen_Unk_ov000_0235cbd8()) {
            ResultsScreen_Exit(next);
            return;
        }
        if (next == 2) {
            WirelessSession_Stop();
            ResultsScreen_ShowLinkLostMessage();
            return;
        }
        ResultsScreen_Exit(next);
    }
}

#pragma thumb reset
