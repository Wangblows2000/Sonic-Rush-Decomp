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

extern UnkTask_022e0f94 *gTaskCurrent;
extern UnkTouch_022e0f94 gTouchState;
extern UnkPad_022e0f94 gPad;
extern s32 data_ov001_0231c3ec;
extern s32 gZoneActSelectResult;

s32 ZoneActSelect_Unk_ov001_022e0820(s32 x, s32 y);
s32 ZoneActSelect_Unk_ov001_022e07e8(s32 x, s32 y);
BOOL ZoneActSelect_Unk_ov001_022e07d0(s32 x, s32 y);
void SysSound_PlayCursorSE(void);
void ZoneActSelect_Unk_ov001_022e08e0(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0968(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e09d0(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0ad0(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0c5c(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0dfc(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0d30(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0884(UnkWork_022e0f94 *w);
void ZoneActSelect_Unk_ov001_022e0858(UnkWork_022e0f94 *w, s32 a);
void SysSound_PlaySE(s32 a);
void ZoneActSelect_Unk_ov001_022e0e6c(void);
void ZoneActSelect_State_FadeOut(void);
void ZoneActSelect_State_Unk_ov001_022e0f08(void);

#pragma thumb on
void ZoneActSelect_State_Select(void) {
    UnkWork_022e0f94 *w = gTaskCurrent->work;
    s32 x;
    s32 y;
    BOOL ok;
    BOOL cancel;
    s32 sel;

    ok = FALSE;
    cancel = FALSE;
    y = -1;
    x = -1;
    if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
    }
    sel = ZoneActSelect_Unk_ov001_022e0820(x, y);
    if (sel >= w->count) {
        sel = -1;
    }
    if (sel < 0) {
        if (gPad.trg & 0x20) {
            if (data_ov001_0231c3ec > 0) {
                sel = data_ov001_0231c3ec - 1;
            }
        } else if (gPad.trg & 0x10) {
            if (data_ov001_0231c3ec < w->count - 1) {
                sel = data_ov001_0231c3ec + 1;
            }
        }
    }
    if (sel >= 0) {
        if (data_ov001_0231c3ec != sel) {
            SysSound_PlayCursorSE();
        }
        data_ov001_0231c3ec = sel;
    }
    sel = ZoneActSelect_Unk_ov001_022e07e8(x, y);
    if (sel < 0) {
        if (gPad.trg & 0x40) {
            if (gZoneActSelectResult > 0) {
                sel = gZoneActSelectResult - 1;
                SysSound_PlayCursorSE();
            }
        } else if (gPad.trg & 0x80) {
            if (gZoneActSelectResult < 2) {
                sel = gZoneActSelectResult + 1;
                SysSound_PlayCursorSE();
            }
        }
    } else {
        ok = TRUE;
    }
    if (sel >= 0 && gZoneActSelectResult != sel) {
        gZoneActSelectResult = sel;
        w->unk4 = 0;
    }
    if (gPad.trg & 1) {
        ok = TRUE;
    }
    if (!ok) {
        if (ZoneActSelect_Unk_ov001_022e07d0(x, y)) {
            cancel = TRUE;
            w->unkdc = 1;
        } else if (gPad.trg & 2) {
            cancel = TRUE;
        }
    }
    ZoneActSelect_Unk_ov001_022e08e0(w);
    ZoneActSelect_Unk_ov001_022e0968(w);
    ZoneActSelect_Unk_ov001_022e09d0(w);
    ZoneActSelect_Unk_ov001_022e0ad0(w);
    ZoneActSelect_Unk_ov001_022e0c5c(w);
    ZoneActSelect_Unk_ov001_022e0dfc(w);
    ZoneActSelect_Unk_ov001_022e0d30(w);
    ZoneActSelect_Unk_ov001_022e0884(w);
    ZoneActSelect_Unk_ov001_022e0858(w, data_ov001_0231c3ec);
    w->unk0++;
    w->unk4++;
    if (cancel) {
        w->unk0 = 0;
        SysSound_PlaySE(0xc);
        gTaskCurrent->func = ZoneActSelect_State_FadeOut;
        return;
    }
    if (ok) {
        w->unk0 = 0;
        w->unkd8 = 1;
        SysSound_PlaySE(0x12);
        ZoneActSelect_Unk_ov001_022e0e6c();
        gTaskCurrent->func = ZoneActSelect_State_Unk_ov001_022e0f08;
    }
}
#pragma thumb reset
