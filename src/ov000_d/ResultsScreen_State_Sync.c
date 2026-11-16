#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235eab8_Sync;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_0235eab8_Task;

extern UnkStruct_0235eab8_Task *gTaskCurrent;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

UnkStruct_0235eab8_Sync *WirelessSession_GetSendBuffer(void);
UnkStruct_0235eab8_Sync *WirelessSession_GetPartnerRecvBuffer(void);
UnkStruct_0235eab8_Sync *WirelessSession_GetOwnRecvBuffer(void);
BOOL func_ov000_0235cbd8(void);
BOOL WirelessSession_Unk_020513a8(void);
void *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void ResultsScreen_ScrollBg_Main(void);
void SysSound_Release(void);
void Stage_FreeZoneSoundData(void);
void SysSound_Init(void);
void SysSound_PlayBGM(s32 a);
void ResultsScreen_State_FadeIn(void);

#pragma thumb on

void ResultsScreen_State_Sync(void) {
    BOOL go = FALSE;
    UnkStruct_0235eab8_Sync *a = WirelessSession_GetSendBuffer();
    UnkStruct_0235eab8_Sync *b = WirelessSession_GetPartnerRecvBuffer();
    UnkStruct_0235eab8_Sync *c = WirelessSession_GetOwnRecvBuffer();

    if (func_ov000_0235cbd8()) {
        if (a->unk3 == 0 && b->unk3 == 0) {
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
            go = TRUE;
        }
    } else {
        if (b->unk3 == 0) {
            a->unk3 = go;
        }
        if (a->unk3 == 0) {
            if (b->unk3 == 1) {
                a->unk3 = 1;
            }
        } else if (a->unk3 == 1) {
            if (b->unk3 == 2) {
                a->unk3 = 2;
            }
        } else if (a->unk3 == 2) {
            if (b->unk3 == 3) {
                go = TRUE;
            }
        }
    }
    if (go) {
        Task_Create(ResultsScreen_ScrollBg_Main, NULL, 0, 0x2000, 0x1000, 0);
        if (WirelessSession_Unk_020513a8()) {
            SysSound_Release();
            Stage_FreeZoneSoundData();
        }
        SysSound_Init();
        SysSound_PlayBGM(3);
        gSaveDataBusy = 1;
        gSystemFlags |= 0x10;
        gTaskCurrent->unk8 = ResultsScreen_State_FadeIn;
    }
}

#pragma thumb reset
