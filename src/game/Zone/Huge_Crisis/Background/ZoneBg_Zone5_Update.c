/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 ZoneBg_Zone5_UpdateScreenScroll();
s32 ZoneBg_Zone5_UpdateParallax();

void ZoneBg_Zone5_Update(s32 p0) {
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 1) != 0) {
        ZoneBg_Zone5_UpdateScreenScroll(p0, 0);
        ZoneBg_Zone5_UpdateParallax(p0);
        return;
    }
    ZoneBg_Zone5_UpdateScreenScroll(p0, 0);
    ZoneBg_Zone5_UpdateScreenScroll(p0, 1);
}
