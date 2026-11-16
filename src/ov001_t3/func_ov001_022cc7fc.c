#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    s32 unk8;
} UnkSub_022cc7fc;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc7fc sub;
} UnkWork_022cc7fc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc7fc *work;
} UnkTask_022cc7fc;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cc7fc;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cc7fc;

extern UnkTask_022cc7fc *data_022b4574;
extern UnkPad_022cc7fc data_022503e0;
extern UnkTouch_022cc7fc data_022b65b4;

void func_ov001_022cc17c(UnkWork_022cc7fc *w);
void func_ov001_022cbf30(UnkWork_022cc7fc *w);
void func_ov001_022cdef8(void);
void func_02052104(s32 a);
BOOL func_ov001_022c8ba8(u16 x, u16 y);
s32 func_ov001_022c8d54(UnkSub_022cc7fc *s);
void func_ov001_022cc33c(void);
void func_ov001_022cc6b8(void);

#pragma thumb on
void func_ov001_022cc7fc(void) {
    UnkWork_022cc7fc *w;
    UnkSub_022cc7fc *sub;
    u16 x;
    u16 y;

    w = data_022b4574->work;
    sub = &w->sub;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            w->unk1c = -1;
            func_ov001_022cdef8();
            data_022b4574->func = func_ov001_022cc33c;
        }
    }
    if (w->unk1c < 0) {
        if (data_022503e0.trg & 2) {
            func_ov001_022cdef8();
            data_022b4574->func = func_ov001_022cc33c;
            func_02052104(1);
        } else if (data_022b65b4.flags & 0x40) {
            x = data_022b65b4.x;
            y = data_022b65b4.y;
            if (x < 0x100 && y < 0xc0 && func_ov001_022c8ba8(x, y)) {
                w->unk1c = 0;
                func_02052104(1);
            }
        }
        sub->unk8 = func_ov001_022c8d54(sub);
        if (sub->unk8 > 0) {
            func_02052104(0);
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 5;
            data_022b4574->func = func_ov001_022cc6b8;
        }
    }
}
#pragma thumb reset
