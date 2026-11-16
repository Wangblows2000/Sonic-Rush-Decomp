/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
s32 PaletteTransfer_Clear();
s32 ScreenTransfer_Clear();
s32 VRAMTransfer_Clear();
s32 TitleDemo_ReleaseModels();
s32 func_ov001_022d6734();
s32 TitleDemo_ReleaseSprites();
s32 TitleDemo_ReleaseAssets();
s32 func_ov001_022d4ff0();

void TitleDemo_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    PaletteTransfer_Clear();
    ScreenTransfer_Clear();
    VRAMTransfer_Clear();
    TitleDemo_ReleaseModels(t0);
    func_ov001_022d6734(t0);
    TitleDemo_ReleaseSprites(t0);
    TitleDemo_ReleaseAssets(t0);
    func_ov001_022d4ff0();
}

#pragma thumb reset
