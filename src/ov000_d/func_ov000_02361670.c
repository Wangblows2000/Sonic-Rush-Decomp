#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02361670_Sync;

UnkStruct_02361670_Sync *func_020514e4(void);
UnkStruct_02361670_Sync *func_02051474(void);
void func_ov000_02360dac(UnkStruct_MenuO *w);
void func_ov000_0236143c(UnkStruct_MenuO *w);
void func_ov000_02361614(void);
void SysSound_StopBGM(s32 a);

#pragma thumb on

void func_ov000_02361670(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    UnkStruct_02361670_Sync *a = func_020514e4();
    UnkStruct_02361670_Sync *b = func_02051474();

    MenuO_Update(w, 0);
    if (b->unk4 >= 3) {
        func_ov000_02360dac(w);
        func_ov000_0236143c(w);
        gTaskCurrent->unk8 = func_ov000_02361614;
        return;
    }
    if (a->unk4 == 0) {
        a->unk4 = 1;
    } else if (a->unk4 == 1) {
        if (b->unk4 == 1) {
            SysSound_StopBGM(0x10);
            w->unk0 = 0;
            a->unk4 = 2;
        }
    } else if (a->unk4 == 2) {
        if (w->unk0 >= 0x10) {
            a->unk4 = 3;
        }
    }
}

#pragma thumb reset
