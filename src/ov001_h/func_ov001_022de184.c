#include <nitro/types.h>

extern u8 data_0208922c[];
extern u8 data_02087d04[];
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void func_ov001_022de0f0(void);

void func_ov001_022de184(void *arg, void (**out)(void)) {
    *(s16 *)(data_0208922c + 0x48) = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, *(s16 *)(data_0208922c + 0x48));
    *(s16 *)(data_02087d04 + 0x48) = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400106c, *(s16 *)(data_02087d04 + 0x48));
    *out = func_ov001_022de0f0;
}
