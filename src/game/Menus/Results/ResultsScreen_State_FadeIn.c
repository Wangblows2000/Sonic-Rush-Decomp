#include <game/Menus/sprobj.h>

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235ea40_Bright;

extern UnkStruct_0235ea40_Bright data_0208926c;
extern UnkStruct_0235ea40_Bright data_02087d44;

s32 ResultsScreen_EaseValue(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void ResultsScreen_State_ShowResult(void);

#pragma thumb on

void ResultsScreen_State_FadeIn(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;

    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    ResultsScreen_DrawModels(w);
    if ((s32)w->unk0 >= 0) {
        s16 v = ResultsScreen_EaseValue(w->unk14, 0, 0x10, w->unk0, 0x1000);
        data_0208926c.unk8 = v;
        data_02087d44.unk8 = v;
    }
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        gTaskCurrent->unk8 = ResultsScreen_State_ShowResult;
    }
}

#pragma thumb reset
