#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_02358444_Sync;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
} UnkStruct_02358444;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02358444 *work;
} UnkStruct_02358444_Task;

extern UnkStruct_02358444_Task *gTaskCurrent;

UnkStruct_02358444_Sync *WirelessSession_GetSendBuffer(void);
UnkStruct_02358444_Sync *WirelessSession_GetPartnerRecvBuffer(void);
UnkStruct_02358444_Sync *WirelessSession_GetOwnRecvBuffer(void);
void PlayerSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void SysSound_PlayBGM(s32 a);
void PlayerSelectMenu_Action_Opening(UnkStruct_02358444_Task *t);

#pragma thumb on

void PlayerSelectMenu_State_Sync(void) {
    UnkStruct_02358444 *w = gTaskCurrent->work;
    UnkStruct_02358444_Sync *a = WirelessSession_GetSendBuffer();
    UnkStruct_02358444_Sync *b = WirelessSession_GetPartnerRecvBuffer();
    UnkStruct_02358444_Sync *c = WirelessSession_GetOwnRecvBuffer();

    if (w->unkC == 1) {
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
            PlayerSelectMenu_StartBrightnessFade(0x10, 0, 0x10, 0);
            SysSound_PlayBGM(2);
            PlayerSelectMenu_Action_Opening(gTaskCurrent);
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
        } else if (a->unk3 == 2 && b->unk3 == 3) {
            PlayerSelectMenu_StartBrightnessFade(0x10, 0, 0x10, 0);
            SysSound_PlayBGM(2);
            PlayerSelectMenu_Action_Opening(gTaskCurrent);
        }
    }
}

#pragma thumb reset
