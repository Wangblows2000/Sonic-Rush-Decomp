#include <ov000_d/menuo.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} UnkStruct_02361bf0_Sync;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

UnkStruct_02361bf0_Sync *func_020514e4(void);
void func_ov000_023610d8(UnkStruct_MenuO *w);
void func_ov000_02360fdc(UnkStruct_MenuO *w);
BOOL func_ov000_0235fa34(UnkStruct_MenuO *w);
void SysSound_PlaySE(s32 a);
void MsgBox_RequestClose(void);
void func_ov000_0236114c(UnkStruct_MenuO *w);
void func_ov000_02360ea4(UnkStruct_MenuO *w, s32 a, s32 b);
void func_ov000_02361b34(void);
BOOL func_ov000_0235f9f0(UnkStruct_MenuO *w);
BOOL func_ov000_0235fa1c(UnkStruct_MenuO *w);
void func_ov000_02360e34(UnkStruct_MenuO *w);
void func_ov000_0235f850(s32 x, s32 y, s32 a, s32 b);
void func_ov000_02361520(void);
BOOL func_ov000_0235fab0(UnkStruct_MenuO *w);
BOOL func_ov000_0235fa70(UnkStruct_MenuO *w);
void func_ov000_023610a4(UnkStruct_MenuO *w);
void func_ov000_02360f98(UnkStruct_MenuO *w);
void func_ov000_02360ee0(UnkStruct_MenuO *w, s32 a, s32 b);
void func_ov000_02360f18(UnkStruct_MenuO *w, s32 a);
void func_ov000_02361914(void);

#pragma thumb on

void func_ov000_02361bf0(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    s32 old;
    s32 n;
    u16 mask;
    s32 q;

    func_ov000_023611f8(w);
    func_ov000_0236119c(w, 0x1000, 1);
    w->unk0++;
    if (w->unk0 == 1 && w->unk8 > 1) {
        func_ov000_023610d8(w);
        func_ov000_02360fdc(w);
    }
    if (func_ov000_0235fa34(w) || w->unk18) {
        UnkStruct_02361bf0_Sync *s;
        w->unk0 = 0;
        SysSound_PlaySE(7);
        s = func_020514e4();
        s->unk0 = w->unkC;
        s->unk1 = 1;
        MsgBox_RequestClose();
        w->unk14 = 0;
        func_ov000_0236114c(w);
        n = w->unkC;
        func_ov000_02360ea4(w, n / 2, n % 2);
        gGameState.unk1C = w->unkC / 2;
        gGameState.unk1D = w->unkC % 2;
        gTaskCurrent->unk8 = func_ov000_02361b34;
        return;
    }
    if (func_ov000_0235f9f0(w) || func_ov000_0235fa1c(w)) {
        w->unk0 = 0;
        SysSound_PlaySE(8);
        func_020514e4()->unk2 = 1;
        if (func_ov000_0235f9f0(w)) {
            func_ov000_02360e34(w);
        }
        func_ov000_0235f850(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = func_ov000_02361520;
        return;
    }
    if (func_ov000_0235fab0(w)) {
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (--n < 0) {
                n = 0xd;
            }
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            func_020514e4()->unk0 = w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            func_ov000_023610a4(w);
            q = n / 2;
            func_ov000_02360ee0(w, q, n % 2);
            if (w->unk10 / 2 != q) {
                func_ov000_02360f18(w, q);
            }
            gTaskCurrent->unk8 = func_ov000_02361914;
        }
        return;
    }
    if (func_ov000_0235fa70(w)) {
        w->unk0 = 0;
        old = w->unkC;
        n = old;
        mask = w->unk24;
        do {
            if (++n > 0xd) {
                n = 0;
            }
        } while (!(mask & (1 << n)));
        if (old != n) {
            w->unk10 = old;
            w->unkC = n;
            func_020514e4()->unk0 = w->unkC;
            KeyframeAnim_SetAnim(&w->pos[0], 6);
            func_ov000_02360f98(w);
            q = n / 2;
            func_ov000_02360ee0(w, q, n % 2);
            if (w->unk10 / 2 != q) {
                func_ov000_02360f18(w, q);
            }
            gTaskCurrent->unk8 = func_ov000_02361914;
        }
    }
}

#pragma thumb reset
