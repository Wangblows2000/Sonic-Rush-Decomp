#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x40 - 4];
} UnkSub_023582e8;

typedef struct {
    u8 pad0[0x32C];
    s32 sel;
    u8 pad330[0x344 - 0x330];
    UnkSub_023582e8 sub[3];
} UnkStruct_023582e8;

typedef struct {
    s16 x;
    s16 y;
} UnkOfs_023582e8;

extern UnkOfs_023582e8 gDisplaySub[4];

void Background_Draw(UnkSub_023582e8 *p);

#pragma thumb on

void PlayerSelectMenu_SetBackgroundSelection(UnkStruct_023582e8 *w, s32 sel, s32 scroll) {
    UnkSub_023582e8 *p;

    if (sel != w->sel) {
        switch (sel) {
        case 0:
            p = &w->sub[0];
            break;
        case 1:
            p = &w->sub[1];
            break;
        case 2:
            p = &w->sub[2];
            break;
        default:
            p = NULL;
            break;
        }
        if (p != NULL) {
            p->flags &= ~0x3f;
            Background_Draw(p);
        }
        w->sel = sel;
    }
    if (sel == 4) {
        gDisplaySub[0].x = 0x100;
    } else {
        scroll = -scroll;
        if (scroll < 0) {
            scroll += ((-scroll) / 0x200 + 1) << 9;
        }
        scroll = scroll % 0x200;
        gDisplaySub[0].x = scroll;
    }
}

#pragma thumb reset
