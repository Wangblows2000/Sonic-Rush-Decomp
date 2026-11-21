#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
} UnkSub_022cc268;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc268 sub;
} UnkWork_022cc268;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc268 *work;
} UnkTask_022cc268;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cc268;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cc268;

extern UnkTask_022cc268 *gTaskCurrent;
extern UnkPad_022cc268 gPad;
extern UnkTouch_022cc268 gTouchState;

void OptionsMenu_UpdateAnims(UnkWork_022cc268 *w);
void OptionsMenu_DrawItems(UnkWork_022cc268 *w);
void SysSound_PlaySE(s32 a);
BOOL OptionsMenu_IsBackButtonTouched(u16 x, u16 y);
void OptionsMenu_State_Unk_ov001_022cc210(void);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc268(void) {
    UnkWork_022cc268 *w;
    UnkSub_022cc268 *sub;
    BOOL done;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    sub = &w->sub;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    done = FALSE;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            w->unk1c = -1;
            done = TRUE;
        }
    }
    if (w->unk1c < 0) {
        if (gPad.trg & 2) {
            done = TRUE;
            SysSound_PlaySE(done);
        } else if (gTouchState.flags & 0x40) {
            x = gTouchState.x;
            y = gTouchState.y;
            if (x < 0x100 && y < 0xc0 && OptionsMenu_IsBackButtonTouched(x, y) && !done) {
                w->unk1c = 0;
                SysSound_PlaySE(1);
            }
        } else if (sub->timer >= 0xb4) {
            done = TRUE;
        }
        if (done) {
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 10;
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc210;
        }
    }
}
#pragma thumb reset
