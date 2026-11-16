#include <nitro/types.h>

extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void func_ov001_022de0f0(void);

void func_ov001_022de184(void *arg, void (**out)(void)) {
    *(s16 *)(gDisplayMain + 0x48) = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, *(s16 *)(gDisplayMain + 0x48));
    *(s16 *)(gDisplaySub + 0x48) = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400106c, *(s16 *)(gDisplaySub + 0x48));
    *out = func_ov001_022de0f0;
}
