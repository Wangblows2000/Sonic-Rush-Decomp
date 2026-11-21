#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 timer;
    s32 timer2;
    s32 pos;
    s32 sel;
} UnkWork_022e8e04;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8e04 *work;
} UnkTask_022e8e04;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e8e04;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e8e04;

extern UnkTask_022e8e04 *gTaskCurrent;
extern UnkPad_022e8e04 gPad;
extern UnkTouch_022e8e04 gTouchState;

void ZoneMapPause_Unk_ov001_022e8b5c(UnkWork_022e8e04 *w);
void ZoneMapPause_DrawItem(UnkWork_022e8e04 *w, s32 n);
void SysSound_PlaySE(s32 a);
BOOL ZoneMapPause_IsTouchOnItem(u16 x, u16 y, s32 n);
void ZoneMapPause_Action_CloseA(s32 a);
void ZoneMapPause_Action_CloseB(s32 a);
void ZoneMapPause_Action_ConfirmA(s32 a);
void ZoneMapPause_Action_ConfirmB(s32 a);

#pragma thumb on
void ZoneMapPause_State_Select(void) {
    UnkWork_022e8e04 *w;
    BOOL ok;
    BOOL cancel;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    ok = FALSE;
    cancel = FALSE;
    ZoneMapPause_Unk_ov001_022e8b5c(w);
    switch (w->mode) {
    case 2:
        ZoneMapPause_DrawItem(w, 0);
        ZoneMapPause_DrawItem(w, 1);
        break;
    case 6:
        ZoneMapPause_DrawItem(w, 2);
        ZoneMapPause_DrawItem(w, 3);
        break;
    }
    w->timer++;
    w->timer2++;
    if (gPad.trg & 0x40) {
        if (w->sel != 0) {
            w->sel = 0;
            w->timer2 = 0;
            SysSound_PlaySE(0x13);
        }
    } else if (gPad.trg & 0x80) {
        if (w->sel != 1) {
            w->sel = 1;
            w->timer2 = 0;
            SysSound_PlaySE(0x13);
        }
    }
    if (gPad.trg & 1) {
        ok = TRUE;
    } else if (gPad.trg & 2) {
        w->sel = -1;
        cancel = TRUE;
    } else if (gPad.trg & 8) {
        if (w->mode == 2) {
            w->sel = -1;
            cancel = TRUE;
        }
    } else if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (ZoneMapPause_IsTouchOnItem(x, y, 0)) {
            w->sel = 0;
            ok = TRUE;
        } else if (ZoneMapPause_IsTouchOnItem(x, y, 1)) {
            ok = TRUE;
            w->sel = 1;
        }
    }
    if (cancel) {
        switch (w->mode) {
        case 2:
            ZoneMapPause_Action_CloseA(0);
            break;
        case 6:
            ZoneMapPause_Action_CloseB(0);
            break;
        }
        SysSound_PlaySE(0xc);
        return;
    }
    if (ok) {
        switch (w->mode) {
        case 2:
            ZoneMapPause_Action_ConfirmA(0);
            break;
        case 6:
            ZoneMapPause_Action_ConfirmB(0);
            break;
        }
        SysSound_PlaySE(0xb);
    }
}
#pragma thumb reset
