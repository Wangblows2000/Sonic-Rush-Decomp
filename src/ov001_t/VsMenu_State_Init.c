#include <nitro/types.h>

typedef struct {
    s32 step;
    s32 unk4;
    s32 mode;
} UnkWork_022dd42c;

typedef struct {
    u8 pad[0x10];
    UnkWork_022dd42c *work;
} UnkTask_022dd42c;

extern UnkTask_022dd42c *gTaskCurrent;

void VsMenu_InitSprites(UnkWork_022dd42c *w);
void VsMenu_Unk_ov001_022ddb00(UnkWork_022dd42c *w);
void VsMenu_Unk_ov001_022dd93c(UnkWork_022dd42c *w);
void VsMenu_InitKeyframeAnims(UnkWork_022dd42c *w);
void VsMenu_Unk_ov001_022dd614(UnkTask_022dd42c *t);
void VsMenu_Unk_ov001_022dd574(UnkTask_022dd42c *t);
void VsMenu_Unk_ov001_022dd534(UnkTask_022dd42c *t);

#pragma thumb on
void VsMenu_State_Init(void) {
    UnkWork_022dd42c *w = gTaskCurrent->work;

    if (w->step == 0) {
        VsMenu_InitSprites(w);
    } else if (w->step == 1) {
        VsMenu_Unk_ov001_022ddb00(w);
    } else if (w->step == 2) {
        VsMenu_Unk_ov001_022dd93c(w);
    } else if (w->step == 3) {
        VsMenu_InitKeyframeAnims(w);
    }
    w->step++;
    if (w->step >= 4) {
        w->step = 0;
        switch (w->mode) {
        case 0:
            VsMenu_Unk_ov001_022dd614(gTaskCurrent);
            break;
        case 1:
            VsMenu_Unk_ov001_022dd574(gTaskCurrent);
            break;
        case 2:
            VsMenu_Unk_ov001_022dd534(gTaskCurrent);
            break;
        }
    }
}
#pragma thumb reset
