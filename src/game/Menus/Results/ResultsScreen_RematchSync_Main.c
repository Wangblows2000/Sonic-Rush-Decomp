#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235e23c_Sync;

typedef struct {
    s32 unk0;
    u8 pad4[0x1C - 4];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern void *gTaskCurrent;

UnkStruct_0235e23c_Sync *WirelessSession_GetSendBuffer(void);
UnkStruct_0235e23c_Sync *WirelessSession_GetPartnerRecvBuffer(void);
UnkStruct_0235e23c_Sync *WirelessSession_GetOwnRecvBuffer(void);
BOOL ResultsScreen_Unk_ov000_0235cbd8(void);
BOOL WirelessSession_IsDownloadPlay(void);
void StageLoader_LoadZoneSoundData(s32 a, u8 b, u8 c);
void Task_Destroy(void *t);
void StageStart_Create(s32 a);

#pragma thumb on

void ResultsScreen_RematchSync_Main(void) {
    UnkStruct_0235e23c_Sync *a = WirelessSession_GetSendBuffer();
    UnkStruct_0235e23c_Sync *b = WirelessSession_GetPartnerRecvBuffer();
    UnkStruct_0235e23c_Sync *c = WirelessSession_GetOwnRecvBuffer();

    if (ResultsScreen_Unk_ov000_0235cbd8()) {
        if (a->unk3 == 0 && b->unk3 == 0) {
            if (WirelessSession_IsDownloadPlay()) {
                StageLoader_LoadZoneSoundData(gGameState.unk0, gGameState.unk1C, gGameState.unk1D);
            }
            a->unk3 = 1;
        } else if (a->unk3 == 1) {
            if (b->unk3 == 1) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2) {
            if (b->unk3 == 2) {
                a->unk3 = 3;
            }
        } else if (c->unk3 >= 3) {
            Task_Destroy(gTaskCurrent);
            StageStart_Create(0);
        }
    } else {
        if (b->unk3 == 0) {
            a->unk3 = 0;
        }
        if (a->unk3 == 0 && b->unk3 == 1) {
            a->unk3 = 1;
        }
        if (a->unk3 == 1) {
            if (b->unk3 == 2) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2 && b->unk3 >= 3) {
            Task_Destroy(gTaskCurrent);
            StageStart_Create(0);
        }
    }
}

#pragma thumb reset
