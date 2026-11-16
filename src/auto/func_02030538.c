/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 GXx_SetMasterBrightness_();
extern u8 data_02087cc8[];
extern u8 data_02087cb4[];
s32 FS_UnloadOverlay();
s32 func_020076cc();

void func_02030538(s32 p0) {
    GXx_SetMasterBrightness_(0x400006c, 0x10);
    GXx_SetMasterBrightness_(0x400106c, 0x10);
    if (*(u32 *)data_02087cc8 != 0) {
        FS_UnloadOverlay(0, *(u32 *)data_02087cb4);
    }
    func_020076cc(((s32)p0 | 0x80000000));
}
