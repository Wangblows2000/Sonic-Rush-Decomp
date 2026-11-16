#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkStruct_02362124_Sync;

UnkStruct_02362124_Sync *func_020514e4(void);
UnkStruct_02362124_Sync *func_02051474(void);
u16 func_ov000_0235f8e8(void);
void func_ov000_0236110c(UnkStruct_MenuO *w);
void func_ov000_02361020(UnkStruct_MenuO *w);
void func_ov000_02361068(UnkStruct_MenuO *w);
void func_ov000_02360f54(UnkStruct_MenuO *w);
void func_ov000_0235f850(s32 x, s32 y, s32 a, s32 b);
void SysSound_Init(void);
void func_02052168(s32 a);
void func_ov000_02361e28(void);

#pragma thumb on

void func_ov000_02362124(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    UnkStruct_02362124_Sync *a = func_020514e4();
    UnkStruct_02362124_Sync *b = func_02051474();
    s32 i;

    w->unk0++;
    if (w->unk0 < 8) {
        return;
    }
    if (w->unk4 != 0) {
        if (a->unk4 == 0) {
            if (b->unk4 == 1) {
                a->unk4 = 1;
                a->unk6 = func_ov000_0235f8e8();
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
            a->unk6 = func_ov000_0235f8e8();
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
            func_ov000_0236110c(w);
            func_ov000_02361020(w);
        } else {
            func_ov000_02361068(w);
            func_ov000_02360f54(w);
        }
    } else {
        func_ov000_02361068(w);
        func_ov000_02360f54(w);
    }
    func_ov000_0235f850(0x10, 0, 0x10, 0);
    SysSound_Init();
    func_02052168(2);
    gTaskCurrent->unk8 = func_ov000_02361e28;
}

#pragma thumb reset
