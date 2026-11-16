#include <ov000_d/menuo.h>

BOOL MsgBox_IsClosed(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL TimeSelectMenu_PollDownloadPlay(UnkStruct_MenuO *w);
void TimeSelectMenu_State_FollowerTransferSync(void);
void TimeSelectMenu_State_FollowerReady(void);

#pragma thumb on

void TimeSelectMenu_State_FollowerConfirmed(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    MenuO_Update(w, 0);
    if (MsgBox_IsClosed()) {
        w->unk0 = 0;
        MsgBox_Create(0xe, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
        if (TimeSelectMenu_PollDownloadPlay(w)) {
            gTaskCurrent->unk8 = TimeSelectMenu_State_FollowerTransferSync;
            return;
        }
        w->unk18 = 0;
        w->unk1C = 0;
        gTaskCurrent->unk8 = TimeSelectMenu_State_FollowerReady;
    }
}

#pragma thumb reset
