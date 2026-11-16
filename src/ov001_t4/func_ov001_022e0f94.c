#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 count;
    u8 pad10[0xd8 - 0x10];
    s32 unkd8;
    s32 unkdc;
} UnkWork_022e0f94;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e0f94 *work;
} UnkTask_022e0f94;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e0f94;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e0f94;

extern UnkTask_022e0f94 *data_022b4574;
extern UnkTouch_022e0f94 data_022b65b4;
extern UnkPad_022e0f94 data_022503e0;
extern s32 data_ov001_0231c3ec;
extern s32 data_ov001_0231c3f0;

s32 func_ov001_022e0820(s32 x, s32 y);
s32 func_ov001_022e07e8(s32 x, s32 y);
BOOL func_ov001_022e07d0(s32 x, s32 y);
void func_02051ebc(void);
void func_ov001_022e08e0(UnkWork_022e0f94 *w);
void func_ov001_022e0968(UnkWork_022e0f94 *w);
void func_ov001_022e09d0(UnkWork_022e0f94 *w);
void func_ov001_022e0ad0(UnkWork_022e0f94 *w);
void func_ov001_022e0c5c(UnkWork_022e0f94 *w);
void func_ov001_022e0dfc(UnkWork_022e0f94 *w);
void func_ov001_022e0d30(UnkWork_022e0f94 *w);
void func_ov001_022e0884(UnkWork_022e0f94 *w);
void func_ov001_022e0858(UnkWork_022e0f94 *w, s32 a);
void func_02052104(s32 a);
void func_ov001_022e0e6c(void);
void func_ov001_022e114c(void);
void func_ov001_022e0f08(void);

#pragma thumb on
void func_ov001_022e0f94(void) {
    UnkWork_022e0f94 *w = data_022b4574->work;
    s32 x;
    s32 y;
    BOOL ok;
    BOOL cancel;
    s32 sel;

    ok = FALSE;
    cancel = FALSE;
    y = -1;
    x = -1;
    if (data_022b65b4.flags & 0x40) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
    }
    sel = func_ov001_022e0820(x, y);
    if (sel >= w->count) {
        sel = -1;
    }
    if (sel < 0) {
        if (data_022503e0.trg & 0x20) {
            if (data_ov001_0231c3ec > 0) {
                sel = data_ov001_0231c3ec - 1;
            }
        } else if (data_022503e0.trg & 0x10) {
            if (data_ov001_0231c3ec < w->count - 1) {
                sel = data_ov001_0231c3ec + 1;
            }
        }
    }
    if (sel >= 0) {
        if (data_ov001_0231c3ec != sel) {
            func_02051ebc();
        }
        data_ov001_0231c3ec = sel;
    }
    sel = func_ov001_022e07e8(x, y);
    if (sel < 0) {
        if (data_022503e0.trg & 0x40) {
            if (data_ov001_0231c3f0 > 0) {
                sel = data_ov001_0231c3f0 - 1;
                func_02051ebc();
            }
        } else if (data_022503e0.trg & 0x80) {
            if (data_ov001_0231c3f0 < 2) {
                sel = data_ov001_0231c3f0 + 1;
                func_02051ebc();
            }
        }
    } else {
        ok = TRUE;
    }
    if (sel >= 0 && data_ov001_0231c3f0 != sel) {
        data_ov001_0231c3f0 = sel;
        w->unk4 = 0;
    }
    if (data_022503e0.trg & 1) {
        ok = TRUE;
    }
    if (!ok) {
        if (func_ov001_022e07d0(x, y)) {
            cancel = TRUE;
            w->unkdc = 1;
        } else if (data_022503e0.trg & 2) {
            cancel = TRUE;
        }
    }
    func_ov001_022e08e0(w);
    func_ov001_022e0968(w);
    func_ov001_022e09d0(w);
    func_ov001_022e0ad0(w);
    func_ov001_022e0c5c(w);
    func_ov001_022e0dfc(w);
    func_ov001_022e0d30(w);
    func_ov001_022e0884(w);
    func_ov001_022e0858(w, data_ov001_0231c3ec);
    w->unk0++;
    w->unk4++;
    if (cancel) {
        w->unk0 = 0;
        func_02052104(0xc);
        data_022b4574->func = func_ov001_022e114c;
        return;
    }
    if (ok) {
        w->unk0 = 0;
        w->unkd8 = 1;
        func_02052104(0x12);
        func_ov001_022e0e6c();
        data_022b4574->func = func_ov001_022e0f08;
    }
}
#pragma thumb reset
