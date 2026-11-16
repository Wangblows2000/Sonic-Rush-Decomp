#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02361670_Sync;

UnkStruct_02361670_Sync *WirelessSession_GetSendBuffer(void);
UnkStruct_02361670_Sync *WirelessSession_GetPartnerRecvBuffer(void);
void TimeSelectMenu_SetTransferSprites(UnkStruct_MenuO *w);
void TimeSelectMenu_StartStageTransfer(UnkStruct_MenuO *w);
void TimeSelectMenu_State_FollowerWaitTransfer(void);
void SysSound_StopBGM(s32 a);

#pragma thumb on

void TimeSelectMenu_State_FollowerTransferSync(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    UnkStruct_02361670_Sync *a = WirelessSession_GetSendBuffer();
    UnkStruct_02361670_Sync *b = WirelessSession_GetPartnerRecvBuffer();

    MenuO_Update(w, 0);
    if (b->unk4 >= 3) {
        TimeSelectMenu_SetTransferSprites(w);
        TimeSelectMenu_StartStageTransfer(w);
        gTaskCurrent->unk8 = TimeSelectMenu_State_FollowerWaitTransfer;
        return;
    }
    if (a->unk4 == 0) {
        a->unk4 = 1;
    } else if (a->unk4 == 1) {
        if (b->unk4 == 1) {
            SysSound_StopBGM(0x10);
            w->unk0 = 0;
            a->unk4 = 2;
        }
    } else if (a->unk4 == 2) {
        if (w->unk0 >= 0x10) {
            a->unk4 = 3;
        }
    }
}

#pragma thumb reset
