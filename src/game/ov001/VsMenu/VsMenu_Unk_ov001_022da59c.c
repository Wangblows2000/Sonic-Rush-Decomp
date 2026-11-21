/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 VsMenu_Unk_ov001_022d9e50();
s32 VsMenu_Unk_ov001_022d9e2c();
s32 VsMenu_Unk_ov001_022d9d98();
s32 VsMenu_Unk_ov001_022d9d64();
s32 VsMenu_Unk_ov001_022d9cf8();
s32 VsMenu_DrawLinkLevelIcon();

void VsMenu_Unk_ov001_022da59c(s32 p0) {
    VsMenu_Unk_ov001_022d9e50(p0, 0x38, 0x98);
    VsMenu_Unk_ov001_022d9e2c(p0, 0x10, 0x48);
    VsMenu_Unk_ov001_022d9d98(p0, 0, 0, 0);
    VsMenu_Unk_ov001_022d9d98(p0, 1, 0, 0);
    VsMenu_Unk_ov001_022d9d64(p0, 0, 0x58, 0x44);
    VsMenu_Unk_ov001_022d9d64(p0, 1, 0x58, 0x74);
    VsMenu_Unk_ov001_022d9cf8(p0, 0, 0x68, 0x43);
    VsMenu_Unk_ov001_022d9cf8(p0, 1, 0x68, 0x73);
    VsMenu_DrawLinkLevelIcon(p0, 0xb0, 8, 0, 0);
}

#pragma thumb reset
