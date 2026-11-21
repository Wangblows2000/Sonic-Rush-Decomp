#include <nitro/types.h>

typedef struct { u16 v[0xb]; } UnkName_02050638;
typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x18];
    u16 unk34;
    u16 unk36;
    UnkName_02050638 names[1]; /* 0x38 */
} UnkWork_02050638;
typedef struct { u8 pad0[0x10]; UnkWork_02050638 *work; } UnkTask_02050638;
extern UnkTask_02050638 *gWirelessLobbyTask;
extern UnkTask_02050638 *gTaskCurrent;
extern u16 gWHPacketSize;
extern u16 gWirelessPartnerName[];
extern s32 gWirelessPartnerNameValid;

s32 WirelessMgr_GetStatus(void);
void Task_Destroy(UnkTask_02050638 *t);
void WirelessLobby_CreateChild(u16 a);
void WirelessMgr_SelectParent(u16 a);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void WirelessSession_Stop(void);

#pragma thumb on
void WirelessLobbyChild_Main(void) {
    UnkTask_02050638 *t = gWirelessLobbyTask;
    UnkWork_02050638 *w = t->work;
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
            WirelessLobby_CreateChild(gWHPacketSize);
            return;
        }
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            break;
        case 2:
            if (w->unk34) {
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (w->unk34 == 0) {
                WirelessLobby_CreateChild(gWHPacketSize);
                return;
            }
            if (w->unk10) {
                WirelessMgr_SelectParent(w->unk36);
                w->unkC = 4;
                w->unk8 = 0;
            }
            break;
        case 4:
            if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                MIi_CpuCopy16(&w->names[w->unk36], gWirelessPartnerName, 0x16);
                gWirelessPartnerNameValid = 1;
            } else if (r == 7) {
                WirelessLobby_CreateChild(gWHPacketSize);
                return;
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
