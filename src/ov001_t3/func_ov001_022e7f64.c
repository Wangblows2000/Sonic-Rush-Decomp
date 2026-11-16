#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1994];
    u8 c[4];
} UnkSub_022e7f64;

typedef struct {
    s32 mode;
    u8 pad4[0x14 - 4];
    s32 state;
    s32 next;
    s32 unk1c;
    u8 pad20[0x74 - 0x20];
    s32 timer;
    u8 pad78[4];
    s32 unk7c;
    u8 pad80[0xa4 - 0x80];
    UnkSub_022e7f64 *sub;
} UnkWork_022e7f64;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e7f64 *work;
} UnkTask_022e7f64;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e7f64;

extern UnkTask_022e7f64 *data_022b4574;
extern UnkPad_022e7f64 data_022503e0;

void func_ov001_022e6dec(UnkWork_022e7f64 *w);
BOOL func_ov001_022e37fc(void);
void func_ov001_022e7728(void);
s32 func_ov001_022e6c90(UnkWork_022e7f64 *w, s32 a, s32 b);
s32 func_ov001_022e6bb4(UnkWork_022e7f64 *w);
s32 func_ov001_022e6998(s32 a, s32 b, s32 c);
void func_ov001_022e794c(void);
void func_ov001_022e3510(UnkWork_022e7f64 *w);
void func_ov001_022e349c(UnkWork_022e7f64 *w);
void func_ov001_022e3840(void *c, s32 a);
void func_ov001_022e3844(void *c, s32 a);
void func_ov001_022e612c(void);
void func_ov001_022e5e60(void);

#pragma thumb on
void func_ov001_022e7f64(void) {
    UnkWork_022e7f64 *w;
    s32 n;

    w = data_022b4574->work;
    func_ov001_022e6dec(w);
    if (func_ov001_022e37fc() && w->timer != 0) {
        w->timer = 0;
        data_022b4574->func = func_ov001_022e7728;
        return;
    }
    n = func_ov001_022e6c90(w, w->state, 0x12);
    if (n == 0x12) {
        n = func_ov001_022e6bb4(w);
    }
    if (n != 0x12 && n != w->state) {
        w->timer = 0;
        w->next = n;
        w->unk1c = func_ov001_022e6998(w->mode, w->state, w->next);
        data_022b4574->func = func_ov001_022e794c;
        return;
    }
    if (w->timer == 0) {
        func_ov001_022e3510(w);
    }
    func_ov001_022e349c(w);
    func_ov001_022e3840(w->sub->c, 0x1000);
    func_ov001_022e3844(w->sub->c, 0);
    if ((data_022503e0.trg & 1) || n == w->state) {
        func_ov001_022e612c();
        return;
    }
    if (data_022503e0.trg & 8) {
        func_ov001_022e5e60();
        return;
    }
    if (w->timer == 0) {
        func_ov001_022e3840(w->sub->c, 0x1000);
        func_ov001_022e3844(w->sub->c, 0);
    }
    w->timer++;
    w->unk7c = 0;
}
#pragma thumb reset
