#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkStruct_02362124_Sync;

UnkStruct_02362124_Sync *WirelessSession_GetSendBuffer(void);
UnkStruct_02362124_Sync *WirelessSession_GetPartnerRecvBuffer(void);
u16 TimeSelectMenu_GetUnlockedStageMask(void);
void TimeSelectMenu_SetSprite78Anim2(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSpriteC0Anim2(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite78Anim4(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSpriteC0Anim4(UnkStruct_MenuO *w);
void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b);
void SysSound_Init(void);
void SysSound_PlayBGM(s32 a);
void TimeSelectMenu_Main(void);

#pragma thumb on

void TimeSelectMenu_State_Sync(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    UnkStruct_02362124_Sync *a = WirelessSession_GetSendBuffer();
    UnkStruct_02362124_Sync *b = WirelessSession_GetPartnerRecvBuffer();
    s32 i;

    w->unk0++;
    if (w->unk0 < 8) {
        return;
    }
    if (w->unk4 != 0) {
        if (a->unk4 == 0) {
            if (b->unk4 == 1) {
                a->unk4 = 1;
                a->unk6 = TimeSelectMenu_GetUnlockedStageMask();
            }
            return;
        }
        if (a->unk4 == 1) {
            if (b->unk4 == 2) {
                a->unk4 = 2;
            }
            return;
        }
    } else {
        if (a->unk4 == 0) {
            a->unk4 = 1;
            a->unk6 = TimeSelectMenu_GetUnlockedStageMask();
            return;
        }
        if (a->unk4 == 1) {
            if (b->unk4 == 1) {
                a->unk4 = 2;
            }
            return;
        }
        if (b->unk4 != 2) {
            return;
        }
    }
    w->unk0 = 0;
    w->unk24 = a->unk6 | b->unk6;
    w->unk8 = 0;
    for (i = 0; i < 0xd; i++) {
        if (w->unk24 & (1 << i)) {
            w->unk8++;
        }
    }
    if (w->unk4 != 0) {
        if (w->unk8 > 1) {
            TimeSelectMenu_SetSprite78Anim2(w);
            TimeSelectMenu_SetSpriteC0Anim2(w);
        } else {
            TimeSelectMenu_SetSprite78Anim4(w);
            TimeSelectMenu_SetSpriteC0Anim4(w);
        }
    } else {
        TimeSelectMenu_SetSprite78Anim4(w);
        TimeSelectMenu_SetSpriteC0Anim4(w);
    }
    TimeSelectMenu_StartBrightnessFade(0x10, 0, 0x10, 0);
    SysSound_Init();
    SysSound_PlayBGM(2);
    gTaskCurrent->unk8 = TimeSelectMenu_Main;
}

#pragma thumb reset
