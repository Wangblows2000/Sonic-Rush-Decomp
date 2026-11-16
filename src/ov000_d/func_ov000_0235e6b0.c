#include <ov000_d/sprobj.h>

BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void func_ov000_0235cdf0(UnkStruct_SprObj *w);
void func_ov000_0235ce04(UnkStruct_SprObj *w);
void func_ov000_0235e340(void);
void func_ov000_0235e8ac(void);

#pragma thumb on

void func_ov000_0235e6b0(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    func_ov000_0235def0(w, w->unk20, 0x1000);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        w->unk0 = 0;
        if (!MsgBox_GetResult()) {
            w->unkC = 1;
            func_ov000_0235cdf0(w);
            gTaskCurrent->unk8 = func_ov000_0235e340;
        } else {
            func_ov000_0235ce04(w);
            func_ov000_0235cfc4(w);
            gTaskCurrent->unk8 = func_ov000_0235e8ac;
        }
    }
}

#pragma thumb reset
