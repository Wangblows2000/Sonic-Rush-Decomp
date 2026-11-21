/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 ZoneBg_Zone3_UpdateScreenScroll();
s32 ZoneBg_Zone3_UpdateParallax();

void ZoneBg_Zone3_Update(u8 *p0) {
    s32 t0 = (*(u32 *)p0 + 0x1);
    *(u32 *)p0 = t0;
    if ((*(u32 *)((u8 *)gCamera + 0xc8) & 1) != 0) {
        ZoneBg_Zone3_UpdateScreenScroll(p0, 0, t0);
        ZoneBg_Zone3_UpdateParallax(p0);
        return;
    }
    ZoneBg_Zone3_UpdateScreenScroll(p0, 0);
    ZoneBg_Zone3_UpdateScreenScroll(p0, 1);
}
