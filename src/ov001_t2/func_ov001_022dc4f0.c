#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
} UnkWork_022dc4f0;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dc4f0 *work;
} UnkTask_022dc4f0;

extern UnkTask_022dc4f0 *data_022b4574;

void func_ov001_022da694(UnkWork_022dc4f0 *w);
void func_ov001_022d9cd4(UnkWork_022dc4f0 *w, s32 a, s32 b);
BOOL func_02050018(void);
void func_ov001_022da41c(UnkWork_022dc4f0 *w);
BOOL func_0205000c(void);
void func_020510e0(void);
void func_ov001_022da33c(UnkWork_022dc4f0 *w);
void func_ov001_022da244(UnkWork_022dc4f0 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dc4f0 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dc4f0 *w, s32 i);
void func_ov001_022dc580(void);
void func_ov001_022dc734(void);

#pragma thumb on
void func_ov001_022dc4f0(void) {
    UnkWork_022dc4f0 *w = data_022b4574->work;

    func_ov001_022da694(w);
    func_ov001_022d9cd4(w, 0, 0);
    w->timer++;
    if (func_02050018()) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da41c(w);
        if (!func_0205000c()) {
            func_020510e0();
            func_ov001_022da33c(w);
            func_ov001_022da244(w, 1);
            func_ov001_022da1cc(w, 1);
            w->unk48 = 1;
            func_ov001_022d983c(w, 1);
            data_022b4574->fn = func_ov001_022dc734;
            return;
        }
        func_ov001_022da33c(w);
        w->unk1c = 0;
        data_022b4574->fn = func_ov001_022dc580;
    }
}
#pragma thumb reset
