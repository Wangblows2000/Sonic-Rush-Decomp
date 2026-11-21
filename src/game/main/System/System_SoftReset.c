/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 GXx_SetMasterBrightness_();
extern u8 gOverlayIsLoaded[];
extern u8 gOverlayLoadedId[];
s32 FS_UnloadOverlay();
s32 OS_ResetSystem();

void System_SoftReset(s32 p0) {
    GXx_SetMasterBrightness_(0x400006c, 0x10);
    GXx_SetMasterBrightness_(0x400106c, 0x10);
    if (*(u32 *)gOverlayIsLoaded != 0) {
        FS_UnloadOverlay(0, *(u32 *)gOverlayLoadedId);
    }
    OS_ResetSystem(((s32)p0 | 0x80000000));
}
