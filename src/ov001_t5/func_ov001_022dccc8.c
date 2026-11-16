#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dccc8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dccc8 *work;
} UnkTask_022dccc8;

extern UnkTask_022dccc8 *data_022b4574;

void func_ov001_022da70c(UnkWork_022dccc8 *w);
void func_02050dfc(void);
void func_ov001_022da33c(UnkWork_022dccc8 *w);
void func_ov001_022da244(UnkWork_022dccc8 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dccc8 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dccc8 *w, s32 a);
void func_ov001_022dd1ec(void);

static inline UnkWork_022dccc8 *Set48_022dccc8(UnkWork_022dccc8 *w, s32 v) {
    w->unk48 = v;
    return w;
}

#pragma thumb on
void func_ov001_022dccc8(void) {
    UnkWork_022dccc8 *w;

    w = data_022b4574->work;
    func_ov001_022da70c(w);
    w->timer++;
    if (w->timer >= 0x1e) {
        w->timer = 0;
        w->unk4 = 0;
        func_02050dfc();
        func_ov001_022da33c(w);
        func_ov001_022da244(w, 1);
        func_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        func_ov001_022d983c(w, 1);
        data_022b4574->func = func_ov001_022dd1ec;
    }
}
#pragma thumb reset
