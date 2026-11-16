#include <ov000_d/sprobj.h>

BOOL func_02050018(void);
BOOL func_0205000c(void);
void func_ov000_0235cdf0(UnkStruct_SprObj *w);
void func_ov000_0235ce04(UnkStruct_SprObj *w);
void func_ov000_0235e340(void);
void func_ov000_0235e8ac(void);

#pragma thumb on

void func_ov000_0235e6b0(void) {
    UnkStruct_SprObj *w = data_022b4574->work;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    func_ov000_0235def0(w, w->unk20, 0x1000);
    w->unk0++;
    if (func_02050018()) {
        w->unk0 = 0;
        if (!func_0205000c()) {
            w->unkC = 1;
            func_ov000_0235cdf0(w);
            data_022b4574->unk8 = func_ov000_0235e340;
        } else {
            func_ov000_0235ce04(w);
            func_ov000_0235cfc4(w);
            data_022b4574->unk8 = func_ov000_0235e8ac;
        }
    }
}

#pragma thumb reset
