#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    s32 unk18;
    s32 unk1C;
} UnkStruct_0236157c;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_0236157c *work;
} UnkStruct_0236157c_Task;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_0236157c_Pad;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
} UnkStruct_0236157c_Touch;

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0236157c_Sync;

extern UnkStruct_0236157c_Task *gTaskCurrent;
extern UnkStruct_0236157c_Pad gPad;
extern UnkStruct_0236157c_Touch gTouchState;

void TimeSelectMenu_DrawLayoutA(UnkStruct_0236157c *w);
void TimeSelectMenu_DrawLayoutB(UnkStruct_0236157c *w, s32 a, s32 b);
UnkStruct_0236157c_Sync *WirelessSession_GetSendBuffer(void);
BOOL TimeSelectMenu_IsPartnerReadyAsFollower(UnkStruct_0236157c *w);
void Wireless_EnableErrorCallback(s32 a);
void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void TimeSelectMenu_State_StartStage(void);

#pragma thumb on

void TimeSelectMenu_State_FollowerReady(void) {
    UnkStruct_0236157c *w = gTaskCurrent->work;

    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, 0);
    w->unk0++;
    if ((gPad.trg & 1) || (gTouchState.flags & 0x40)) {
        w->unk1C = 1;
        WirelessSession_GetSendBuffer()->unk3 = 1;
    }
    if (TimeSelectMenu_IsPartnerReadyAsFollower(w)) {
        w->unk18 = 1;
    }
    if ((w->unk18 && w->unk1C) || w->unk0 >= 0xc8) {
        w->unk0 = 0;
        Wireless_EnableErrorCallback(0);
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = TimeSelectMenu_State_StartStage;
    }
}

#pragma thumb reset
