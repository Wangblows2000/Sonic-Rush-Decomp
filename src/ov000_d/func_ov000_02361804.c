#include <ov000_d/menuo.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

s32 func_ov000_0235fbcc(UnkStruct_MenuO *w);
void func_ov000_02360ee0(UnkStruct_MenuO *w, s32 a, s32 b);
void func_ov000_02360f18(UnkStruct_MenuO *w, s32 a);
BOOL func_ov000_0235faf8(UnkStruct_MenuO *w);
void func_ov000_0235f850(s32 x, s32 y, s32 a, s32 b);
void func_ov000_02361520(void);
BOOL func_ov000_0235fba0(UnkStruct_MenuO *w);
void func_0205004c(void);
void func_ov000_02361744(void);

#pragma thumb on

void func_ov000_02361804(void) {
    UnkStruct_MenuO *w = gTaskCurrent->work;
    s32 n;

    MenuO_Update(w, 0);
    n = func_ov000_0235fbcc(w);
    if (n != w->unkC) {
        s32 q;
        w->unk10 = w->unkC;
        w->unkC = n;
        q = n / 2;
        func_ov000_02360ee0(w, q, n % 2);
        func_ov000_02360f18(w, q);
        if (q != w->unk10 / 2) {
            func_02040ec0(&w->pos[0], 6);
        }
    }
    if (func_ov000_0235faf8(w)) {
        w->unk0 = 0;
        func_ov000_0235f850(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = func_ov000_02361520;
        return;
    }
    if (func_ov000_0235fba0(w)) {
        w->unk0 = 0;
        data_022c4560.unk1C = w->unkC / 2;
        data_022c4560.unk1D = w->unkC % 2;
        func_0205004c();
        gTaskCurrent->unk8 = func_ov000_02361744;
    }
}

#pragma thumb reset
