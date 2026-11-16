#include <ov000_d/menuo.h>

BOOL func_ov000_0235fa34(UnkStruct_MenuO *w);
BOOL func_02040cf4(UnkStruct_MenuOPos *p);
void func_ov000_02361bf0(void);

#pragma thumb on

void func_ov000_02361914(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    MenuO_Update(w, 1);
    if (func_ov000_0235fa34(w)) {
        w->unk18 = 1;
    }
    if (func_02040cf4(&w->pos[0])) {
        w->unk0 = 0;
        gTaskCurrent->unk8 = func_ov000_02361bf0;
    }
}

#pragma thumb reset
