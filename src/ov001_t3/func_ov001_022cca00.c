#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    s32 unk10;
    s32 unk14;
} UnkSub_022cca00;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cca00 sub;
} UnkWork_022cca00;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cca00 *work;
} UnkTask_022cca00;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cca00;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cca00;

extern UnkTask_022cca00 *gTaskCurrent;
extern UnkPad_022cca00 gPad;
extern UnkTouch_022cca00 gTouchState;

void func_ov001_022cc17c(UnkWork_022cca00 *w);
void func_ov001_022cbf30(UnkWork_022cca00 *w);
void func_ov001_022cdef8(void);
void SysSound_PlaySE(s32 a);
BOOL func_ov001_022c8ba8(u16 x, u16 y);
s32 func_ov001_022c8d54(UnkSub_022cca00 *s);
void func_ov001_022cc33c(void);
void func_ov001_022cdd28(void);
void func_ov001_022cc998(void);
BOOL func_ov001_022c8b78(u16 x, u16 y);
BOOL func_ov001_022c8b60(u16 x, u16 y);

#pragma thumb on
void func_ov001_022cca00(void) {
    UnkWork_022cca00 *w;
    UnkSub_022cca00 *sub;
    BOOL ok;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    sub = &w->sub;
    sub->unk10 = 0;
    sub->unk14 = 0;
    sub->unk8 = 0;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            w->unk1c = -1;
            func_ov001_022cdd28();
        }
    }
    if (w->unk1c < 0) {
        ok = FALSE;
        if (gPad.trg & 2) {
            SysSound_PlaySE(1);
            func_ov001_022cdd28();
        } else if (gTouchState.flags & 0x40) {
            x = gTouchState.x;
            y = gTouchState.y;
            if (x < 0x100 && y < 0xc0) {
                if (func_ov001_022c8ba8(x, y)) {
                    w->unk1c = 0;
                    SysSound_PlaySE(1);
                } else if (func_ov001_022c8b78(x, y)) {
                    ok = TRUE;
                    sub->unkc = 0;
                    SysSound_PlaySE(0);
                } else if (func_ov001_022c8b60(x, y)) {
                    ok = TRUE;
                    sub->unkc = 1;
                    SysSound_PlaySE(0);
                }
            }
        } else if (gPad.trg & 1) {
            ok = TRUE;
            SysSound_PlaySE(0);
        } else if (gPad.trg & 0x20) {
            if (sub->unkc != 0) {
                sub->unkc = 0;
                w->unk14 = 0;
                SysSound_PlaySE(4);
            }
        } else if (gPad.trg & 0x10) {
            if (sub->unkc != 1) {
                sub->unkc = 1;
                w->unk14 = 0;
                SysSound_PlaySE(4);
            }
        }
        if (ok) {
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 2;
            gTaskCurrent->func = func_ov001_022cc998;
        }
    }
}
#pragma thumb reset
