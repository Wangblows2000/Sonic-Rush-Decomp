#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

s32 TimeSelectMenu_GetChooserChoice(UnkStruct_MenuO *w);
void func_ov000_02360ee0(UnkStruct_MenuO *w, s32 a, s32 b);
void func_ov000_02360f18(UnkStruct_MenuO *w, s32 a);
BOOL TimeSelectMenu_HasChooserQuit(UnkStruct_MenuO *w);
void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void TimeSelectMenu_State_ExitToPlayerSelect(void);
BOOL TimeSelectMenu_HasChooserDecided(UnkStruct_MenuO *w);
void MsgBox_RequestClose(void);
void TimeSelectMenu_State_FollowerConfirmed(void);

#pragma thumb on

void TimeSelectMenu_State_FollowChooser(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    s32 n;

    MenuO_Update(w, 0);
    n = TimeSelectMenu_GetChooserChoice(w);
    if (n != w->unkC) {
        s32 q;
        w->unk10 = w->unkC;
        w->unkC = n;
        q = n / 2;
        func_ov000_02360ee0(w, q, n % 2);
        func_ov000_02360f18(w, q);
        if (q != w->unk10 / 2) {
            KeyframeAnim_SetAnim(&w->pos[0], 6);
        }
    }
    if (TimeSelectMenu_HasChooserQuit(w)) {
        w->unk0 = 0;
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = TimeSelectMenu_State_ExitToPlayerSelect;
        return;
    }
    if (TimeSelectMenu_HasChooserDecided(w)) {
        w->unk0 = 0;
        gGameState.unk1C = w->unkC / 2;
        gGameState.unk1D = w->unkC % 2;
        MsgBox_RequestClose();
        gTaskCurrent->unk8 = TimeSelectMenu_State_FollowerConfirmed;
    }
}

#pragma thumb reset
