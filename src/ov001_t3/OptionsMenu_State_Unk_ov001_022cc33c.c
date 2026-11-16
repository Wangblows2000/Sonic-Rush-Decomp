#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 state;
} UnkSub_022cc33c;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x1ec - 0x1c];
    UnkSub_022cc33c sub;
} UnkWork_022cc33c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc33c *work;
} UnkTask_022cc33c;

extern UnkTask_022cc33c *gTaskCurrent;

void OptionsMenu_UpdateAnims(UnkWork_022cc33c *w);
void OptionsMenu_DrawItems(UnkWork_022cc33c *w);
BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void OptionsMenu_StopWireless(UnkSub_022cc33c *s);
void OptionsMenu_Unk_ov001_022cdd28(void);
void OptionsMenu_State_Unk_ov001_022ccb5c(void);
void OptionsMenu_State_Unk_ov001_022cca00(void);
void OptionsMenu_State_Unk_ov001_022cc998(void);
void OptionsMenu_State_Unk_ov001_022cc8ec(void);
void OptionsMenu_State_Unk_ov001_022cc7fc(void);
void OptionsMenu_State_Unk_ov001_022cc6b8(void);
void OptionsMenu_State_Unk_ov001_022cc588(void);
void OptionsMenu_State_Unk_ov001_022cc4a0(void);
void OptionsMenu_State_Unk_ov001_022cc268(void);
void OptionsMenu_State_Unk_ov001_022cc210(void);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc33c(void) {
    UnkWork_022cc33c *w;
    UnkSub_022cc33c *sub;

    w = gTaskCurrent->work;
    sub = &w->sub;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (!MsgBox_IsClosed()) return;
    if (!MsgBox_GetResult()) {
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        OptionsMenu_StopWireless(sub);
        OptionsMenu_Unk_ov001_022cdd28();
        return;
    }
    switch (sub->state) {
    case 0:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022ccb5c;
        return;
    case 1:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cca00;
        return;
    case 2:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc998;
        return;
    case 3:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc8ec;
        return;
    case 4:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc7fc;
        return;
    case 5:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc6b8;
        return;
    case 7:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc588;
        return;
    case 8:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc4a0;
        return;
    case 9:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc268;
        return;
    case 10:
        gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc210;
        return;
    case 6:
    default:
        OptionsMenu_StopWireless(sub);
        OptionsMenu_Unk_ov001_022cdd28();
        break;
    }
}
#pragma thumb reset
