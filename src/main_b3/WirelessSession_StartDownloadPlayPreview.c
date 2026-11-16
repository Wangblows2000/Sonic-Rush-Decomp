#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; } UnkWork_02050c88;
typedef struct { u8 pad0[0x10]; UnkWork_02050c88 *work; } UnkTask_02050c88;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u32 pad0[8]; } UnkCfg_02050c88;
extern UnkTask_02050c88 *data_022c4638;
extern s32 data_022c463c;
extern UnkStruct_0207ad9c gWirelessSession;

void Wireless_EnableErrorCallback(s32 a);
void WirelessSession_ForceStop(void);
void NetPacket_Init(s32 a);
UnkTask_02050c88 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void WirelessLobbyPreview_Main(void);
void WirelessLobbyDownloadPlay_Destructor(void);
void DownloadPlay_GetSonicPreview(UnkCfg_02050c88 *c);
void DownloadPlay_GetBlazePreview(UnkCfg_02050c88 *c);
void WirelessMgr_StartDownloadPlayParent(UnkCfg_02050c88 *c, s32 a);

#pragma thumb on
void WirelessSession_StartDownloadPlayPreview(s32 mode) {
    UnkWork_02050c88 *w;
    UnkCfg_02050c88 cfg;

    Wireless_EnableErrorCallback(0);
    WirelessSession_ForceStop();
    NetPacket_Init(0);
    data_022c463c = 0;
    data_022c4638 = Task_Create(WirelessLobbyPreview_Main, WirelessLobbyDownloadPlay_Destructor, 0, 0x1000, 0, sizeof(UnkWork_02050c88));
    w = data_022c4638->work;
    w->unk0 = 2;
    w->unk4 = 1;
    w->unk8 = 0;
    w->unkC = 0;
    gWirelessSession.unk0 = w->unk0;
    gWirelessSession.unk2 = w->unk4;
    if (mode == 0)
        DownloadPlay_GetSonicPreview(&cfg);
    else
        DownloadPlay_GetBlazePreview(&cfg);
    WirelessMgr_StartDownloadPlayParent(&cfg, 0);
}
#pragma thumb reset
