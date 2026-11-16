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

extern UnkTask_022cd558 *gTaskCurrent;
extern UnkPad_022cd558 gPad;
extern UnkTouch_022cd558 gTouchState;

void OptionsMenu_UpdateAnims(UnkWork_022cd558 *w);
void OptionsMenu_DrawItems(UnkWork_022cd558 *w);
void OptionsMenu_Unk_ov001_022cdd28(void);
void SysSound_PlaySE(s32 se);
void OptionsMenu_Unk_ov001_022c8924(void *p, s32 a);
BOOL OptionsMenu_IsBackButtonTouched(u32 x, u32 y);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cd558(void) {
    UnkWork_022cd558 *w = gTaskCurrent->work;
    u32 x, y;

    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            OptionsMenu_Unk_ov001_022cdd28();
        }
    }
    if (w->unk1c >= 0) return;
    if (w->chosen != -1) {
        if (w->unk18 < 0x10) return;
        OptionsMenu_Unk_ov001_022cdd28();
        w->chosen = -1;
        return;
    }
    if (gPad.rep & 0x20) {
        if (w->sel != 0) {
            w->sel = 0;
            w->unk14 = 0;
            SysSound_PlaySE(4);
        }
    } else if (gPad.rep & 0x10) {
        if (w->sel != 1) {
            w->sel = 1;
            w->unk14 = 0;
            SysSound_PlaySE(4);
        }
    }
    if (gPad.trg & 1) {
        w->chosen = w->sel;
        w->unk18 = 0;
        switch (w->sel) {
        case 0:
            OptionsMenu_Unk_ov001_022c8924(w->unk3ac, 0);
            break;
        case 1:
            OptionsMenu_Unk_ov001_022c8924(w->unk3ac, 1);
            break;
        }
        SysSound_PlaySE(0);
        return;
    }
    if (gPad.trg & 2) {
        SysSound_PlaySE(1);
        OptionsMenu_Unk_ov001_022cdd28();
        return;
    }
    if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x < 0x100 && y < 0xc0) {
            if (OptionsMenu_IsBackButtonTouched(x, y)) {
                SysSound_PlaySE(1);
                w->unk1c = 0;
            }
            if (x >= 0x20 && x < 0x7a && y >= 0x50 && y < 0x70) {
                w->sel = 0;
                w->chosen = w->sel;
                w->unk18 = 0;
                OptionsMenu_Unk_ov001_022c8924(w->unk3ac, 0);
                SysSound_PlaySE(0);
            }
            if (x >= 0x88 && x < 0xe2 && y >= 0x50 && y < 0x70) {
                w->sel = 1;
                w->chosen = w->sel;
                w->unk18 = 0;
                OptionsMenu_Unk_ov001_022c8924(w->unk3ac, 1);
                SysSound_PlaySE(0);
            }
        }
    }
}
#pragma thumb reset
