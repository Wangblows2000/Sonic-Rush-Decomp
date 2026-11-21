#include <ov000_d/sprobj.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235e94c_Sync;

UnkStruct_0235e94c_Sync *WirelessSession_GetSendBuffer(void);
s32 ResultsScreen_EaseValue(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void ResultsScreen_Main_CloseWindow(void);

#pragma thumb on

void ResultsScreen_State_ShowResult(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;
    s32 t;

    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    if (w->unk0 == 0) {
        switch (w->unk20) {
        case 0:
            if (w->unk18 == 0) {
                ResultsScreen_SetModelAnimA(w, 1);
                ResultsScreen_SetModelAnimB(w, 2);
            } else {
                ResultsScreen_SetModelAnimA(w, 2);
                ResultsScreen_SetModelAnimB(w, 1);
            }
            break;
        case 1:
            if (w->unk18 == 0) {
                ResultsScreen_SetModelAnimA(w, 2);
                ResultsScreen_SetModelAnimB(w, 1);
            } else {
                ResultsScreen_SetModelAnimA(w, 1);
                ResultsScreen_SetModelAnimB(w, 2);
            }
            break;
        case 2:
            ResultsScreen_SetModelAnimA(w, 2);
            ResultsScreen_SetModelAnimB(w, 2);
            break;
        }
    }
    ResultsScreen_DrawModels(w);
    t = w->unk0 - 0x38;
    if (t >= 0) {
        ResultsScreen_DrawWinLoseSprites(w, w->unk20, ResultsScreen_EaseValue(0, 0x1000, 8, t, 0x1000));
    }
    w->unk0++;
    if (w->unk0 >= 0x40) {
        UnkStruct_0235e94c_Sync *s = WirelessSession_GetSendBuffer();
        w->unk0 = 0;
        s->unk3 = 0;
        ResultsScreen_EnableLineScrollSplit(w);
        gTaskCurrent->unk8 = ResultsScreen_Main_CloseWindow;
    }
}

#pragma thumb reset
