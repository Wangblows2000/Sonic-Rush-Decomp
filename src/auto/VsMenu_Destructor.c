/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_ov001_0231be10[];
s32 func_02051838();
s32 func_ov001_022dd774();
s32 func_ov001_022dd770();
s32 VsMenu_ReleaseSprites();
s32 VsMenu_ReleaseAssets();
extern u8 gVsMenuTask[];

void VsMenu_Destructor(u8 *p0) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    u32 t1 = *(u32 *)data_ov001_0231be10;
    if (t1 != 0) {
        func_02051838(t1);
    }
    func_ov001_022dd774(t0);
    func_ov001_022dd770(t0);
    VsMenu_ReleaseSprites(t0);
    VsMenu_ReleaseAssets(t0);
    *(u32 *)gVsMenuTask = 0;
}

#pragma thumb reset
