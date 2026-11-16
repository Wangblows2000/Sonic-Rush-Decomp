#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkSub_022cc588;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x1ec - 0x1c];
    UnkSub_022cc588 sub;
} UnkWork_022cc588;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022cc588 *work;
} UnkTask_022cc588;

extern UnkTask_022cc588 *gTaskCurrent;

void OptionsMenu_UpdateAnims(UnkWork_022cc588 *w);
void OptionsMenu_DrawItems(UnkWork_022cc588 *w);
s32 OptionsMenu_GetChildCount(UnkSub_022cc588 *s);
void OptionsMenu_Unk_ov001_022c8d4c(UnkSub_022cc588 *s);
void MsgBox_RequestClose(void);
BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void OptionsMenu_State_Unk_ov001_022cc4a0(void);
void OptionsMenu_State_Unk_ov001_022cc6b8(void);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc588(void) {
    UnkWork_022cc588 *w = gTaskCurrent->work;
    UnkSub_022cc588 *s = &w->sub;

    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.unk0++;
    w->unk14++;
    w->unk18++;
    s->unk8 = OptionsMenu_GetChildCount(s);
    if (s->unk8 == 0) {
        MsgBox_RequestClose();
    }
    if (MsgBox_IsClosed()) {
        if (!MsgBox_GetResult() && s->unk8 > 0) {
            s->unk0 = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            s->unk4 = 8;
            gTaskCurrent->fn = OptionsMenu_State_Unk_ov001_022cc4a0;
            OptionsMenu_Unk_ov001_022c8d4c(s);
            return;
        }
        s->unk4 = 5;
        gTaskCurrent->fn = OptionsMenu_State_Unk_ov001_022cc6b8;
    }
}
#pragma thumb reset
