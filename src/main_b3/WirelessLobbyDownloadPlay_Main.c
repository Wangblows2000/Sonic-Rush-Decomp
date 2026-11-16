#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    void *unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x18];
    u16 unk34;
    u16 unk36;
} UnkWork_02050510;
typedef struct { u8 pad0[0x10]; UnkWork_02050510 *work; } UnkTask_02050510;
typedef struct { u8 pad0[0x14]; u16 unk14; } UnkStruct_022c4644;
extern UnkTask_02050510 *data_022c4638;
extern UnkStruct_022c4644 data_022c4644;
extern s32 data_022c4640;

s32 WirelessMgr_GetStatus(void);
s32 WirelessMgr_GetErrorCode(void);
s32 WirelessMgr_GetPlayerCount(void);
void Wireless_OnError(void);
void WirelessSession_ForceStop(void);
void WirelessSession_RestartDownloadPlay(void *a);
void WirelessMgr_RequestStart(void);
u16 func_02050d60(s32 a, void *b, s32 c);
void WirelessSession_Stop(void);

#pragma thumb on
void WirelessLobbyDownloadPlay_Main(void) {
    UnkTask_02050510 *t = data_022c4638;
    UnkWork_02050510 *w = t->work;
    s32 st;
    s32 r;

    if (t == NULL)
        st = 8;
    else
        st = w->unkC;
    if (st < 7) {
        r = WirelessMgr_GetStatus();
        if (r == 7 || st == 0 || w->unk18) {
            if (WirelessMgr_GetErrorCode() == 3) {
                Wireless_OnError();
            } else {
                void *p = w->unk10;
                WirelessSession_ForceStop();
                WirelessSession_RestartDownloadPlay(p);
            }
            return;
        }
        w->unk36 = WirelessMgr_GetPlayerCount() - 1;
        if (w->unk36 > w->unk34)
            w->unk36 = 0;
        if (WirelessMgr_GetErrorCode() == 2)
            w->unk36 = 0;
        switch (st) {
        case 0:
            break;
        case 1:
            w->unkC = 2;
            w->unk8 = 0;
            break;
        case 2:
            if (w->unk36 != 0 && r == 2) {
                w->unkC = 3;
                w->unk8 = 0;
            }
            break;
        case 3:
            if (w->unk36 == 0) {
                w->unkC = 2;
                w->unk8 = 0;
            } else if (w->unk14) {
                WirelessMgr_RequestStart();
                w->unkC = 4;
                w->unk8 = 0;
                data_022c4644.unk14 = func_02050d60(0, &data_022c4644, 10);
                data_022c4640 = 1;
            }
            break;
        case 4:
            if (w->unk36 == 0) {
                w->unkC = 2;
                w->unk8 = 0;
            } else if (r == 6) {
                w->unkC = 5;
                w->unk8 = 0;
                w->unk14 = 0;
                w->unk18 = 0;
                WirelessSession_Stop();
                return;
            }
            break;
        case 5:
        case 6:
        case 7:
            return;
        }
        w->unk8++;
        w->unk14 = 0;
        w->unk18 = 0;
    }
}
#pragma thumb reset
