#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
    u8 pad10[4];
    s32 cur;
    s32 sel;
    s32 unk1c;
    s32 unk20;
    s32 unk24;
    u8 pad28[0x1a4 - 0x28];
    s32 unk1a4;
} UnkSub_022cd05c;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x18];
    UnkSub_022cd05c sub;
} UnkWork_022cd05c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cd05c *work;
} UnkTask_022cd05c;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022cd05c;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cd05c;

typedef struct {
    u8 pad0[0x29];
    u8 unk29;
    u8 unk2a;
    u8 unk2b;
    u8 unk2c;
    u8 unk2d;
    u16 unk2e;
} UnkEntry_022cd05c;

extern UnkTask_022cd05c *gTaskCurrent;
extern UnkPad_022cd05c gPad;
extern UnkTouch_022cd05c gTouchState;
extern UnkEntry_022cd05c *data_ov001_0231f048;
extern s32 data_ov001_0231f04c;
extern s32 data_02250464;
extern s32 gSESoundHandle;
extern s32 data_02250450;

void func_ov001_022cc17c(UnkWork_022cd05c *w);
void func_ov001_022c8e1c(UnkSub_022cd05c *s);
void func_ov001_022cbf30(UnkWork_022cd05c *w);
void func_ov001_022cdd28(void);
BOOL func_ov001_022c8ba8(u16 x, u16 y);
void func_ov001_022c9260(UnkSub_022cd05c *s);
void SysSound_Init(void);
void func_02052168(s32 a);
void SysSound_PlaySE(s32 a);
BOOL func_ov001_022c9310(UnkSub_022cd05c *s);
void NNS_SndPlayerStopSeqBySeqArcIdx(u8 a, u16 b, s32 c);
BOOL func_ov001_022c88d8(void);
void SoundTest_PlayTrack(UnkSub_022cd05c *s);
void SoundTest_LoadTrack(UnkSub_022cd05c *s, s32 a);
void func_ov001_022c90a8(UnkSub_022cd05c *s, s32 a, s32 b);

#pragma thumb on
void OptionsMenu_State_SoundTest(void) {
    UnkWork_022cd05c *w;
    UnkSub_022cd05c *sub;
    BOOL ok;
    BOOL cancel;
    BOOL left;
    BOOL right;
    BOOL busy;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    sub = &w->sub;
    func_ov001_022cc17c(w);
    if (sub->unkc != 0) {
        func_ov001_022c8e1c(sub);
    }
    func_ov001_022cbf30(w);
    w->unk14++;
    w->unk18++;
    sub->unk1c++;
    sub->unk20++;
    sub->unk24++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            func_ov001_022cdd28();
        }
    }
    if (w->unk1c >= 0) {
        return;
    }
    ok = FALSE;
    cancel = FALSE;
    left = FALSE;
    right = FALSE;
    if (gPad.rep & 0x20) {
        left = TRUE;
    } else if (gPad.rep & 0x10) {
        right = TRUE;
    }
    if (gPad.trg & 1) {
        ok = TRUE;
    } else if (gPad.trg & 2) {
        cancel = TRUE;
    } else if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x < 0x100 && y < 0xc0) {
            if (func_ov001_022c8ba8(x, y)) {
                func_ov001_022c9260(sub);
                w->unk1c = 0;
                SysSound_Init();
                func_02052168(0);
                SysSound_PlaySE(1);
            }
            if (x >= 0x58 && x < 0x86 && y >= 0x6a && y < 0x8a) {
                ok = TRUE;
            }
            if (x >= 0x83 && x < 0xb1 && y >= 0x6a && y < 0x8a && sub->unk0 != 0) {
                cancel = TRUE;
            }
            if (x >= 0x45 && x < 0x65 && y >= 0x48 && y < 0x68) {
                left = TRUE;
            }
            if (x >= 0x9a && x < 0xba && y >= 0x48 && y < 0x68) {
                right = TRUE;
            }
        }
    }
    if (sub->unk0 != 0) {
        UnkEntry_022cd05c *e = &data_ov001_0231f048[sub->sel];
        switch (e->unk2a) {
        case 0:
            if (e->unk29 == 0 && e->unk2b == 14) {
                if (sub->unk1a4 == 0) {
                    cancel = TRUE;
                }
            } else if (data_02250464 == 0) {
                cancel = TRUE;
            }
            break;
        case 1:
            if (gSESoundHandle == 0) {
                cancel = TRUE;
            }
            break;
        case 2:
            if (data_02250450 == 0) {
                cancel = TRUE;
            }
            break;
        }
    }
    if (func_ov001_022c9310(sub) && !ok && !cancel && sub->sel == 100 && sub->unk20 == 0x4b) {
        UnkEntry_022cd05c *e = &data_ov001_0231f048[sub->sel];
        NNS_SndPlayerStopSeqBySeqArcIdx(e->unk2c, e->unk2e, 0x1e);
    }
    busy = func_ov001_022c88d8();
    if (ok) {
        if (sub->sel != sub->cur) {
            sub->unk24 = 0;
        }
        if (sub->sel == sub->cur && func_ov001_022c9310(sub)) {
            SoundTest_PlayTrack(sub);
        } else {
            if (func_ov001_022c9310(sub)) {
                func_ov001_022c9260(sub);
            }
            SoundTest_LoadTrack(sub, sub->cur);
            func_ov001_022c90a8(sub, sub->cur, sub->sel);
        }
        sub->unk20 = 0;
        return;
    }
    if (cancel) {
        if (sub->unk0 != 0) {
            func_ov001_022c9260(sub);
            sub->unk20 = 0;
            if (sub->sel != sub->cur) {
                sub->unk24 = 0;
                sub->sel = sub->cur;
            }
            return;
        } else {
            func_ov001_022c9260(sub);
            func_ov001_022cdd28();
            SysSound_Init();
            func_02052168(0);
            SysSound_PlaySE(1);
            return;
        }
    }
    if (left) {
        do {
            sub->cur--;
            if (sub->cur < 0) {
                sub->cur = data_ov001_0231f04c - 1;
            }
        } while (!busy && data_ov001_0231f048[sub->cur].unk2d == 1);
        sub->unk1c = 0;
        func_ov001_022c90a8(sub, sub->cur, sub->sel);
        if (sub->unk0 == 0) {
            sub->sel = sub->cur;
            sub->unk24 = 0;
        }
        return;
    }
    if (right) {
        do {
            sub->cur++;
            if (sub->cur >= data_ov001_0231f04c) {
                sub->cur = 0;
            }
        } while (!busy && data_ov001_0231f048[sub->cur].unk2d == 1);
        sub->unk1c = 0;
        func_ov001_022c90a8(sub, sub->cur, sub->sel);
        if (sub->unk0 == 0) {
            sub->sel = sub->cur;
            sub->unk24 = 0;
        }
    }
}
#pragma thumb reset
