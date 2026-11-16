#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    s32 unk14;
} UnkSub_022cc4a0;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc4a0 sub;
} UnkWork_022cc4a0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc4a0 *work;
} UnkTask_022cc4a0;

extern UnkTask_022cc4a0 *gTaskCurrent;

void OptionsMenu_UpdateAnims(UnkWork_022cc4a0 *w);
void OptionsMenu_DrawItems(UnkWork_022cc4a0 *w);
s32 OptionsMenu_Unk_ov001_022c8d44(UnkSub_022cc4a0 *s);
s32 OptionsMenu_Unk_ov001_022c8d0c(UnkSub_022cc4a0 *s);
BOOL OptionsMenu_HasWirelessError(UnkSub_022cc4a0 *s);
void OptionsMenu_CreateWfsHost(UnkSub_022cc4a0 *s);
void OptionsMenu_StopWireless(UnkSub_022cc4a0 *s);
void SysSound_PlaySE(s32 a);
void OptionsMenu_State_Unk_ov001_022cc268(void);
void OptionsMenu_State_Unk_ov001_022cca00(void);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc4a0(void) {
    UnkWork_022cc4a0 *w;
    UnkSub_022cc4a0 *sub;
    BOOL done;
    BOOL ok;

    w = gTaskCurrent->work;
    sub = &w->sub;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    done = FALSE;
    ok = FALSE;
    if (sub->unk10 == 0) {
        sub->unk10 = OptionsMenu_Unk_ov001_022c8d44(sub);
        if (sub->unk10 != 0) {
            if (OptionsMenu_HasWirelessError(sub)) {
                ok = TRUE;
            } else {
                OptionsMenu_CreateWfsHost(sub);
            }
        }
    } else if (sub->unk14 == 0) {
        sub->unk14 = OptionsMenu_Unk_ov001_022c8d0c(sub);
        if (sub->unk14 != 0) {
            if (OptionsMenu_HasWirelessError(sub)) {
                ok = TRUE;
            } else {
                done = TRUE;
            }
        }
    } else {
        done = TRUE;
    }
    if (done) {
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        sub->unk4 = 9;
        OptionsMenu_StopWireless(sub);
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc268;
        return;
    }
    if (ok) {
        SysSound_PlaySE(6);
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        sub->unk4 = 1;
        OptionsMenu_StopWireless(sub);
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cca00;
    }
}
#pragma thumb reset
