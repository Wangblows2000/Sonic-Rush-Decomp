#include <game/Menus/mdl.h>

#pragma thumb on

void PlayerSelectMenu_SetModelSelection(UnkStruct_Mdl *w, s32 sel) {
    s32 both;

    if (w->mode == 2) {
        both = TRUE;
    } else {
        both = FALSE;
    }
    w->prevSel = w->sel;
    w->sel = sel;
    if (w->mode != 1) {
        w->slot[0].flag = both;
        switch (sel) {
        case 0:
            if (both) {
                w->slot[0].cur = 0;
                w->slot[0].frame = 0;
                w->slot[0].angOfs = 0;
            } else {
                w->slot[0].cur = 0;
                w->slot[0].frame = 0;
                w->slot[0].angOfs = 0xb1c7;
            }
            w->slot[0].unk120 |= 4;
            break;
        case 1:
            if (both) {
                w->slot[0].cur = 1;
                w->slot[0].frame = 1;
                w->slot[0].angOfs = 0;
            } else {
                w->slot[0].cur = 1;
                w->slot[0].frame = 1;
                w->slot[0].angOfs = 0xb1c7;
            }
            w->slot[0].unk120 &= ~4;
            break;
        }
    }
    if (w->mode != 0) {
        w->slot[1].flag = both;
        switch (sel) {
        case 0:
            if (both) {
                w->slot[1].cur = 2;
                w->slot[1].frame = 2;
                w->slot[1].angOfs = 0;
            } else {
                w->slot[1].cur = 4;
                w->slot[1].frame = 4;
                w->slot[1].angOfs = 0xae38;
            }
            w->slot[1].unk120 |= 4;
            break;
        case 1:
            if (both) {
                w->slot[1].cur = 3;
                w->slot[1].frame = 3;
                w->slot[1].angOfs = 0;
            } else {
                w->slot[1].cur = 5;
                w->slot[1].frame = 5;
                w->slot[1].angOfs = 0xae38;
            }
            w->slot[1].unk120 &= ~4;
            break;
        }
    }
}

#pragma thumb reset
