#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x30 - 0x20];
    s32 sel;
    s32 chosen;
    u8 pad38[0x3ac - 0x38];
    void *unk3ac;
} UnkWork_022cd558;

typedef struct {
    u8 pad[0x10];
    UnkWork_022cd558 *work;
} UnkTask_022cd558;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022cd558;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cd558;

extern UnkTask_022cd558 *data_022b4574;
extern UnkPad_022cd558 data_022503e0;
extern UnkTouch_022cd558 data_022b65b4;

void func_ov001_022cc17c(UnkWork_022cd558 *w);
void func_ov001_022cbf30(UnkWork_022cd558 *w);
void func_ov001_022cdd28(void);
void func_02052104(s32 se);
void func_ov001_022c890c(void *p, s32 a);
BOOL func_ov001_022c8ba8(u32 x, u32 y);

#pragma thumb on
void func_ov001_022cd3c8(void) {
    UnkWork_022cd558 *w = data_022b4574->work;
    u32 x, y;

    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            func_ov001_022cdd28();
        }
    }
    if (w->unk1c >= 0) return;
    if (w->chosen != -1) {
        if (w->unk18 < 0x10) return;
        func_ov001_022cdd28();
        w->chosen = -1;
        return;
    }
    if (data_022503e0.rep & 0x20) {
        if (w->sel != 0) {
            w->sel = 0;
            w->unk14 = 0;
            func_02052104(4);
        }
    } else if (data_022503e0.rep & 0x10) {
        if (w->sel != 1) {
            w->sel = 1;
            w->unk14 = 0;
            func_02052104(4);
        }
    }
    if (data_022503e0.trg & 1) {
        w->chosen = w->sel;
        w->unk18 = 0;
        switch (w->sel) {
        case 0:
            func_ov001_022c890c(w->unk3ac, 1);
            break;
        case 1:
            func_ov001_022c890c(w->unk3ac, 0);
            break;
        }
        func_02052104(0);
        return;
    }
    if (data_022503e0.trg & 2) {
        func_02052104(1);
        func_ov001_022cdd28();
        return;
    }
    if (data_022b65b4.flags & 0x40) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        if (x < 0x100 && y < 0xc0) {
            if (func_ov001_022c8ba8(x, y)) {
                w->unk1c = 0;
                func_02052104(1);
            }
            if (x >= 0x20 && x < 0x7a && y >= 0x50 && y < 0x70) {
                w->sel = 0;
                w->chosen = w->sel;
                w->unk18 = 0;
                func_ov001_022c890c(w->unk3ac, 1);
                func_02052104(0);
            }
            if (x >= 0x88 && x < 0xe2 && y >= 0x50 && y < 0x70) {
                w->sel = 1;
                w->chosen = w->sel;
                w->unk18 = 0;
                func_ov001_022c890c(w->unk3ac, 0);
                func_02052104(0);
            }
        }
    }
}
#pragma thumb reset
