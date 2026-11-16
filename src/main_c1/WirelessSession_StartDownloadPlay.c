#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0xe];
    u16 name[10];  /* 0x1e */
    u16 unk32;
    u16 unk34;
    u16 unk36;
} UnkWork_020509a0;
typedef struct { u8 pad0[0x10]; UnkWork_020509a0 *work; } UnkTask_020509a0;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u8 pad0[0x18]; u16 unk18; } UnkStruct_02072cb8;
typedef struct { u32 pad0[6]; u8 unk18; u8 pad19[7]; } UnkCfg_020509a0;
extern UnkTask_020509a0 *data_022c4638;
extern UnkStruct_0207ad9c gWirelessSession;
extern u16 data_02072cbc[];
extern UnkStruct_02072cb8 gGameLanguage;
extern s32 data_022c4640;

void Task_Destroy(UnkTask_020509a0 *t);
UnkTask_020509a0 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void WirelessLobbyDownloadPlay_Main(void);
void WirelessLobbyDownloadPlay_Destructor(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void Wireless_EnableErrorCallback(s32 a);
void DownloadPlay_GetContest(UnkCfg_020509a0 *c);
void DownloadPlay_GetSonicPreview(UnkCfg_020509a0 *c);
void WirelessMgr_StartDownloadPlayParent(UnkCfg_020509a0 *c, s32 a);

#pragma thumb on
UnkWork_020509a0 *WirelessSession_StartDownloadPlay(s32 mode) {
    UnkWork_020509a0 *w;
    vu16 zero;
    s32 i;
    u16 n;
    UnkCfg_020509a0 cfg;

    if (data_022c4638 != NULL) {
        Task_Destroy(data_022c4638);
        data_022c4638 = NULL;
    }
    data_022c4638 = Task_Create(WirelessLobbyDownloadPlay_Main, WirelessLobbyDownloadPlay_Destructor, 0, 0x1000, 0, sizeof(UnkWork_020509a0));
    w = data_022c4638->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_020509a0));
    w->unk0 = 2;
    w->unk4 = 1;
    gWirelessSession.unk0 = w->unk0;
    gWirelessSession.unk2 = w->unk4;
    MIi_CpuCopy16(data_02072cbc, w->name, 0x14);
    n = gGameLanguage.unk18;
    for (i = n; i < 10; i++)
        w->name[i] = 0x20;
    w->unk32 = i = n;
    Wireless_EnableErrorCallback(0);
    switch (mode) {
    case 0:
        DownloadPlay_GetContest(&cfg);
        break;
    case 1:
    default:
        DownloadPlay_GetSonicPreview(&cfg);
        break;
    }
    WirelessMgr_StartDownloadPlayParent(&cfg, 0);
    w->unk34 = cfg.unk18 - 1;
    w->unk36 = 0;
    w->unkC = 1;
    data_022c4640 = 0;
    return w;
}
#pragma thumb reset
