#include <ov000_d/menuo.h>

BOOL MsgBox_IsClosed(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL TimeSelectMenu_Unk_ov000_0235faf0(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite30Anim0(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSprite1Anim(UnkStruct_MenuO *w, s32 a, s32 b);
void TimeSelectMenu_SetSprite78Anim4(UnkStruct_MenuO *w);
void TimeSelectMenu_SetSpriteC0Anim4(UnkStruct_MenuO *w);
void TimeSelectMenu_State_ChooserTransferSync(void);
void TimeSelectMenu_State_ChooserReady(void);

#pragma thumb on

void TimeSelectMenu_State_ChooserConfirmed(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    TimeSelectMenu_DrawLayoutA(w);
    TimeSelectMenu_DrawLayoutB(w, 0x1000, 1);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        s32 n;
        w->unk0 = 0;
        TimeSelectMenu_SetSprite30Anim0(w);
        n = w->unkC;
        TimeSelectMenu_SetSprite1Anim(w, n / 2, n % 2);
        TimeSelectMenu_SetSprite78Anim4(w);
        TimeSelectMenu_SetSpriteC0Anim4(w);
        MsgBox_Create(0xe, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
        if (TimeSelectMenu_Unk_ov000_0235faf0(w)) {
            gTaskCurrent->unk8 = TimeSelectMenu_State_ChooserTransferSync;
            return;
        }
        w->unk18 = 0;
        w->unk1C = 0;
        gTaskCurrent->unk8 = TimeSelectMenu_State_ChooserReady;
    }
}

#pragma thumb reset
