#include <nitro/types.h>

#define DISP3DCNT (*(vu16 *)0x04000060)
#define FOG_OFFSET (*(vu16 *)0x0400035c)

void G3X_SetFog(BOOL enable, u32 fogMode, u32 fogSlope, s32 fogOffset)
{
    if (enable) {
        FOG_OFFSET = (u16)fogOffset;
        DISP3DCNT = (u16)((DISP3DCNT & ~0x3f40) | ((fogSlope << 8) | (fogMode << 6) | 0x80));
    } else {
        DISP3DCNT &= (u16)~0x3080;
    }
}
