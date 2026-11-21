#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u16 unk1C;
    u16 unk1E[0xb];
    u16 unk34;
    u16 unk36[0xb];
} UnkWork_02050764;
typedef struct { u8 pad0[0x10]; UnkWork_02050764 *work; } UnkTask_02050764;
extern UnkTask_02050764 *gWirelessLobbyTask;
extern UnkTask_02050764 *gTaskCurrent;
extern u16 gWHPacketSize;
extern u16 gWirelessPartnerName[];
extern s32 gWirelessPartnerNameValid;

s32 WirelessMgr_GetStatus(void);
void Task_Destroy(UnkTask_02050764 *t);
void WirelessLobby_CreateParent(u16 a);
BOOL WirelessLobby_GetFirstChild(UnkWork_02050764 *w);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void WirelessMgr_UpdateGameInfo(u16 *buf, s32 size, s32 a);
void WirelessMgr_RequestStart(void);
u16 WH_GetCurrentAid(void);
void WirelessSession_Stop(void);

#pragma thumb on
void WirelessLobbyParent_Main(void) {
    UnkTask_02050764 *t = gWirelessLobbyTask;
    UnkWork_02050764 *w = t->work;
    s32 st;
    s32 r;

    if (t == NULL)
        st = 7;
    else
        st = w->unkC;
    if (st < 6) {
        r = WirelessMgr_GetStatus();
        if (w->unk18) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        if (r == 7 || st == 0 || w->unk14) {
            WirelessLobby_CreateParent(gWHPacketSize);
            return;
        }
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            w->unk8 = 0;
            break;
        case 2:
            if (WirelessLobby_GetFirstChild(w)) {
                u16 buf[0xc];
                MIi_CpuCopy16(w->unk1E, buf, 0x16);
                buf[0xb] = 2;
                WirelessMgr_UpdateGameInfo(buf, 0x18, 1);
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (r != 2) {
                WirelessLobby_CreateParent(gWHPacketSize);
            } else if (w->unk10) {
                WirelessMgr_RequestStart();
                w->unk1C = WH_GetCurrentAid();
                w->unkC = 4;
                w->unk8 = 0;
            }
            break;
        case 4:
            if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                MIi_CpuCopy16(w->unk36, gWirelessPartnerName, 0x16);
                gWirelessPartnerNameValid = 1;
            } else if (r != 2) {
                WirelessLobby_CreateParent(gWHPacketSize);
            }
            break;
        case 5:
            w->unkC = 6;
            w->unk8 = 0;
            w->unk10 = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            WirelessSession_Stop();
            return;
        case 6:
            break;
        }
        w->unk8++;
        w->unk10 = 0;
        w->unk14 = 0;
        w->unk18 = 0;
    }
}
#pragma thumb reset
