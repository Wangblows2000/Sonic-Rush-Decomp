#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    s32 unk8;
} UnkSub_022cc6b8;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc6b8 sub;
} UnkWork_022cc6b8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc6b8 *work;
} UnkTask_022cc6b8;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cc6b8;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cc6b8;

extern UnkTask_022cc6b8 *gTaskCurrent;
extern UnkPad_022cc6b8 gPad;
extern UnkTouch_022cc6b8 gTouchState;

void OptionsMenu_UpdateAnims(UnkWork_022cc6b8 *w);
void OptionsMenu_DrawItems(UnkWork_022cc6b8 *w);
void OptionsMenu_ShowMsgBox13(void);
void SysSound_PlaySE(s32 a);
BOOL OptionsMenu_IsBackButtonTouched(u16 x, u16 y);
s32 OptionsMenu_GetChildCount(UnkSub_022cc6b8 *s);
void OptionsMenu_State_Unk_ov001_022cc33c(void);
void OptionsMenu_State_Unk_ov001_022cc7fc(void);
void OptionsMenu_State_Unk_ov001_022cc628(void);
BOOL OptionsMenu_IsCenterButtonTouched(u16 x, u16 y);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc6b8(void) {
    UnkWork_022cc6b8 *w;
    UnkSub_022cc6b8 *sub;
    BOOL ok;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    sub = &w->sub;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            w->unk1c = -1;
            OptionsMenu_ShowMsgBox13();
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc33c;
        }
    }
    if (w->unk1c < 0) {
        ok = FALSE;
        if (gPad.trg & 2) {
            OptionsMenu_ShowMsgBox13();
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc33c;
            SysSound_PlaySE(1);
        } else if (gTouchState.flags & 0x40) {
            x = gTouchState.x;
            y = gTouchState.y;
            if (x < 0x100 && y < 0xc0) {
                if (OptionsMenu_IsBackButtonTouched(x, y)) {
                    w->unk1c = 0;
                    SysSound_PlaySE(1);
                } else if (OptionsMenu_IsCenterButtonTouched(x, y)) {
                    ok = TRUE;
                    SysSound_PlaySE(0);
                }
            }
        } else if (gPad.trg & 8) {
            ok = TRUE;
            SysSound_PlaySE(0);
        }
        sub->unk8 = OptionsMenu_GetChildCount(sub);
        if (sub->unk8 == 0) {
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 4;
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc7fc;
            return;
        }
        if (ok) {
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 6;
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc628;
        }
    }
}
#pragma thumb reset
