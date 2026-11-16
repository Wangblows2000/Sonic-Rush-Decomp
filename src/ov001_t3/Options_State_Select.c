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

extern UnkTask_022cdabc *gTaskCurrent;
extern UnkPad_022cdabc gPad;
extern UnkTouch_022cdabc gTouchState;

void OptionsMenu_UpdateAnims(UnkWork_022cdabc *w);
void OptionsMenu_DrawItems(UnkWork_022cdabc *w);
void Options_Unk_ov001_022cb708(UnkWork_022cdabc *w, s32 a, s32 b);
void SysSound_PlaySE(s32 a);
void Options_Action_Exit(void);
s32 Options_Unk_ov001_022c8bc0(UnkWork_022cdabc *w, u16 x, u16 y);
void Options_Unk_ov001_022cdd7c(void);
void Options_Unk_ov001_022cdf20(s32 a, s32 b);

#pragma thumb on
void Options_State_Select(void) {
    UnkWork_022cdabc *w;
    BOOL ok;
    s32 old;
    s32 r;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->timer++;
    ok = FALSE;
    old = w->sel;
    if (gPad.rep & 0x40) {
        w->sel = old - 1;
        if (w->sel < 0) w->sel = w->count - 1;
        w->timer = 0;
        Options_Unk_ov001_022cb708(w, w->sel, old);
        SysSound_PlaySE(4);
    } else if (gPad.rep & 0x80) {
        w->sel = old + 1;
        if (w->sel >= w->count) w->sel = 0;
        w->timer = 0;
        Options_Unk_ov001_022cb708(w, w->sel, old);
        SysSound_PlaySE(4);
    }
    if (gPad.trg & 1) {
        ok = TRUE;
        old = w->sel;
    } else if (gPad.trg & 2) {
        SysSound_PlaySE(1);
        Options_Action_Exit();
    } else if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x < 0x100 && y < 0xc0) {
            r = Options_Unk_ov001_022c8bc0(w, x, y);
            if (r < 6) {
                old = w->sel;
                if (old != r) {
                    w->sel = r;
                    w->timer = 0;
                }
                ok = TRUE;
            } else if (r == 6) {
                SysSound_PlaySE(1);
                Options_Unk_ov001_022cdd7c();
            }
        }
    }
    if (ok) {
        Options_Unk_ov001_022cdf20(w->sel, old);
    }
}
#pragma thumb reset
