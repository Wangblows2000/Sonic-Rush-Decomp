#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 sel;
} UnkStruct_022da614;

void VsMenu_Unk_ov001_022d9e50(UnkStruct_022da614 *p, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkStruct_022da614 *p, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9b3c(UnkStruct_022da614 *p, s32 i, s32 x, s32 y);
void VsMenu_DrawCursor(UnkStruct_022da614 *p, s32 x, s32 y);
s32 WirelessLobbyChild_GetParentCount(void);
s32 WirelessLobbyChild_GetParentLinkLevel(s32 i);
void VsMenu_DrawLinkLevelIcon(UnkStruct_022da614 *p, s32 x, s32 y, s32 a, s32 b);

#pragma thumb on
void VsMenu_DrawParentList(UnkStruct_022da614 *p) {
    s32 y;
    s32 i;
    s32 max;

    VsMenu_Unk_ov001_022d9e50(p, 0x38, 0x98);
    VsMenu_Unk_ov001_022d9e2c(p, 0x10, 0x48);
    y = 0x20;
    for (i = 0; i < 8; i++) {
        VsMenu_Unk_ov001_022d9b3c(p, i, 0x38, y);
        if (p->sel == i) {
            VsMenu_DrawCursor(p, 0x38, y);
        }
        y += 0x11;
    }
    max = 0;
    for (i = 0; i < WirelessLobbyChild_GetParentCount(); i++) {
        if (max < WirelessLobbyChild_GetParentLinkLevel(i)) {
            max = WirelessLobbyChild_GetParentLinkLevel(i);
        }
    }
    VsMenu_DrawLinkLevelIcon(p, 0xb0, 8, 0, max);
}
#pragma thumb reset
