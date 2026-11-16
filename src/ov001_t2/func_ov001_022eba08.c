#include <nitro/types.h>

typedef struct {
    s32 state;
    u8 pad4[0xd0 - 4];
    s32 unkd0;
} UnkWork_022eba08;

typedef struct UnkTask_022eba08 {
    u8 pad0[8];
    void (*fn)(struct UnkTask_022eba08 *);
    u8 padc[4];
    UnkWork_022eba08 *work;
} UnkTask_022eba08;

extern UnkTask_022eba08 *data_022b4574;

void func_ov001_022e9708(UnkWork_022eba08 *w, s32 a, s32 *x, s32 *y);
void func_ov001_022e9ba8(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022ea80c(UnkWork_022eba08 *w);
void func_ov001_022ea458(UnkWork_022eba08 *w, s32 x, s32 y);
void func_ov001_022ea7c4(UnkWork_022eba08 *w);
void func_ov001_022ea6e4(UnkWork_022eba08 *w);
void func_02052104(s32 a);
void func_ov001_022eab78(UnkTask_022eba08 *t);

#pragma thumb on
void func_ov001_022eba08(UnkTask_022eba08 *t) {
    UnkWork_022eba08 *w;
    s32 x;
    s32 y;

    if (t == NULL) {
        t = data_022b4574;
    }
    w = t->work;
    w->state = 0;
    func_ov001_022e9708(w, w->unkd0, &x, &y);
    func_ov001_022e9ba8(0, 0x10, 8, 8);
    func_ov001_022ea80c(w);
    func_ov001_022ea458(w, x, y);
    func_ov001_022ea7c4(w);
    func_ov001_022ea6e4(w);
    func_02052104(0);
    t->fn = func_ov001_022eab78;
}
#pragma thumb reset
