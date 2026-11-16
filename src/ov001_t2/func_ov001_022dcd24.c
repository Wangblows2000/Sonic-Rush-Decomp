#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dcd24;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dcd24 *work;
} UnkTask_022dcd24;

extern UnkTask_022dcd24 *data_022b4574;

void func_ov001_022da70c(UnkWork_022dcd24 *w);
BOOL func_02050d30(void);
void func_0205004c(void);
void func_02050dfc(void);
BOOL func_02050018(void);
void func_ov001_022da41c(UnkWork_022dcd24 *w);
BOOL func_0205000c(void);
void func_ov001_022da33c(UnkWork_022dcd24 *w);
void func_ov001_022da244(UnkWork_022dcd24 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dcd24 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dcd24 *w, s32 i);
void func_ov001_022dd1ec(void);
void func_ov001_022dcdbc(void);

#pragma thumb on
void func_ov001_022dcd24(void) {
    UnkWork_022dcd24 *w = data_022b4574->work;

    func_ov001_022da70c(w);
    w->timer++;
    if (func_02050d30()) {
        func_0205004c();
    }
    if (func_02050018()) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da41c(w);
        if (!func_02050d30() && !func_0205000c()) {
            func_02050dfc();
            func_ov001_022da33c(w);
            func_ov001_022da244(w, 1);
            func_ov001_022da1cc(w, 1);
            w->unk48 = 1;
            func_ov001_022d983c(w, 1);
            data_022b4574->fn = func_ov001_022dd1ec;
            return;
        }
        func_ov001_022da33c(w);
        data_022b4574->fn = func_ov001_022dcdbc;
    }
}
#pragma thumb reset
