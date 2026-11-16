#include <ov000_d/menuo.h>

BOOL func_02050018(void);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
BOOL func_ov000_0235faf0(UnkStruct_MenuO *w);
void func_ov000_02361670(void);
void func_ov000_0236157c(void);

#pragma thumb on

void func_ov000_02361744(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;

    MenuO_Update(w, 0);
    if (func_02050018()) {
        w->unk0 = 0;
        func_02050078(0xe, 3, 5, 1, 0, 0, 0xe, 0x200010);
        w->unk14 = 1;
        if (func_ov000_0235faf0(w)) {
            gTaskCurrent->unk8 = func_ov000_02361670;
            return;
        }
        w->unk18 = 0;
        w->unk1C = 0;
        gTaskCurrent->unk8 = func_ov000_0236157c;
    }
}

#pragma thumb reset
