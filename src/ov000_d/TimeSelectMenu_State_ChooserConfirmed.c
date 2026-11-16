#include <ov000_d/menuo.h>

BOOL MsgBox_IsClosed(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL func_ov000_0235faf0(UnkStruct_MenuO *w);
void func_ov000_02361174(UnkStruct_MenuO *w);
void func_ov000_02360ee0(UnkStruct_MenuO *w, s32 a, s32 b);
void func_ov000_02361068(UnkStruct_MenuO *w);
void func_ov000_02360f54(UnkStruct_MenuO *w);
void TimeSelectMenu_State_ChooserTransferSync(void);
void TimeSelectMenu_State_ChooserReady(void);

#pragma thumb on

void TimeSelectMenu_State_ChooserConfirmed(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    func_ov000_023611f8(w);
    func_ov000_0236119c(w, 0x1000, 1);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        s32 n;
        w->unk0 = 0;
        func_ov000_02361174(w);
        n = w->unkC;
        func_ov000_02360ee0(w, n / 2, n % 2);
        func_ov000_02361068(w);
        func_ov000_02360f54(w);
        MsgBox_Create(0xe, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
        if (func_ov000_0235faf0(w)) {
            gTaskCurrent->unk8 = TimeSelectMenu_State_ChooserTransferSync;
            return;
        }
        w->unk18 = 0;
        w->unk1C = 0;
        gTaskCurrent->unk8 = TimeSelectMenu_State_ChooserReady;
    }
}

#pragma thumb reset
