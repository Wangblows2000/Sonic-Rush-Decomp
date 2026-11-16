#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0xC - 4];
    s32 unkC;
} UnkStruct_02361478;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_02361478 *work;
} UnkStruct_02361478_Task;

extern UnkStruct_02361478_Task *gTaskCurrent;

void *WirelessSession_GetSendBuffer(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MsgBox_Destroy(void);
void Task_DestroyByTag(s32 a);
BOOL WirelessSession_IsDownloadPlay(void);
void Wireless_EnableErrorCallback(s32 a);
void StageStart_Create(s32 a);
void SysSound_Release(void);
void Game_StartVsStage(u8 a, u8 b);
void TimeSelectMenu_DrawLayoutA(UnkStruct_02361478 *w);
void TimeSelectMenu_DrawLayoutB(UnkStruct_02361478 *w, s32 a, s32 b);

#pragma thumb on

void TimeSelectMenu_State_StartStage(void) {
    UnkStruct_02361478 *w = gTaskCurrent->work;

    if (w->unk0 == 8) {
        void *s = WirelessSession_GetSendBuffer();
        vu16 zero = 0;
        MIi_CpuClear16(zero, s, 0x40);
    }
    if (w->unk0 >= 0x10) {
        u8 a;
        s32 n;
        u8 b;
        w->unk0 = 0;
        n = w->unkC;
        a = n / 2;
        b = n % 2;
        MsgBox_Destroy();
        Task_DestroyByTag(0x1000);
        if (WirelessSession_IsDownloadPlay()) {
            Wireless_EnableErrorCallback(1);
            StageStart_Create(0);
            return;
        }
        SysSound_Release();
        Game_StartVsStage(a, b);
        return;
    }
    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, 0);
    w->unk0++;
}

#pragma thumb reset
