#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 sel;
    u8 pad8[4];
    s32 count;
    u8 pad10[4];
    s32 timer;
} UnkWork_022cdabc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cdabc *work;
} UnkTask_022cdabc;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022cdabc;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cdabc;

extern UnkTask_022cdabc *data_022b4574;
extern UnkPad_022cdabc data_022503e0;
extern UnkTouch_022cdabc data_022b65b4;

void func_ov001_022cc17c(UnkWork_022cdabc *w);
void func_ov001_022cbf30(UnkWork_022cdabc *w);
void func_ov001_022cb708(UnkWork_022cdabc *w, s32 a, s32 b);
void func_02052104(s32 a);
void func_ov001_022cdcf4(void);
s32 func_ov001_022c8bc0(UnkWork_022cdabc *w, u16 x, u16 y);
void func_ov001_022cdd7c(void);
void func_ov001_022cdf20(s32 a, s32 b);

#pragma thumb on
void func_ov001_022cdabc(void) {
    UnkWork_022cdabc *w;
    BOOL ok;
    s32 old;
    s32 r;
    u16 x;
    u16 y;

    w = data_022b4574->work;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->timer++;
    ok = FALSE;
    old = w->sel;
    if (data_022503e0.rep & 0x40) {
        w->sel = old - 1;
        if (w->sel < 0) w->sel = w->count - 1;
        w->timer = 0;
        func_ov001_022cb708(w, w->sel, old);
        func_02052104(4);
    } else if (data_022503e0.rep & 0x80) {
        w->sel = old + 1;
        if (w->sel >= w->count) w->sel = 0;
        w->timer = 0;
        func_ov001_022cb708(w, w->sel, old);
        func_02052104(4);
    }
    if (data_022503e0.trg & 1) {
        ok = TRUE;
        old = w->sel;
    } else if (data_022503e0.trg & 2) {
        func_02052104(1);
        func_ov001_022cdcf4();
    } else if (data_022b65b4.flags & 0x40) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        if (x < 0x100 && y < 0xc0) {
            r = func_ov001_022c8bc0(w, x, y);
            if (r < 6) {
                old = w->sel;
                if (old != r) {
                    w->sel = r;
                    w->timer = 0;
                }
                ok = TRUE;
            } else if (r == 6) {
                func_02052104(1);
                func_ov001_022cdd7c();
            }
        }
    }
    if (ok) {
        func_ov001_022cdf20(w->sel, old);
    }
}
#pragma thumb reset
