#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    s32 unk5c;
    u8 pad60[0x3ac - 0x60];
    void *next;
} UnkWork_022cdf20;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022cdf20 *work;
} UnkTask_022cdf20;

extern UnkTask_022cdf20 *gTaskCurrent;

void Options_Unk_ov001_022cc044(UnkWork_022cdf20 *w);
void Options_Unk_ov001_022cbac0(UnkWork_022cdf20 *w, s32 a, s32 b);
void Options_Unk_ov001_022cdc8c(void *p);
void Options_Unk_ov001_022cdc74(void *p);
void Options_Unk_ov001_022cdc5c(void *p);
void Options_Unk_ov001_022cdc28(void *p);
void Options_Unk_ov001_022cdc10(void *p);
void Options_Unk_ov001_022cdbf8(void *p);
void SysSound_PlaySE(s32 a);
void OptionsMenu_State_EnterItem(void);

#pragma thumb on
void Options_Unk_ov001_022cdf20(s32 sel, s32 b) {
    UnkWork_022cdf20 *w = gTaskCurrent->work;

    Options_Unk_ov001_022cc044(w);
    Options_Unk_ov001_022cbac0(w, sel, b);
    switch (sel) {
    case 0:
        Options_Unk_ov001_022cdc8c(w->next);
        break;
    case 1:
        Options_Unk_ov001_022cdc74(w->next);
        break;
    case 2:
        Options_Unk_ov001_022cdc5c(w->next);
        break;
    case 3:
        Options_Unk_ov001_022cdc28(w->next);
        w->unk5c = 0;
        break;
    case 4:
        Options_Unk_ov001_022cdc10(w->next);
        break;
    case 5:
        Options_Unk_ov001_022cdbf8(w->next);
        break;
    }
    SysSound_PlaySE(0);
    gTaskCurrent->fn = OptionsMenu_State_EnterItem;
}
#pragma thumb reset
