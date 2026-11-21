/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ZoneFx_Zone2_StopVAlarms();
s32 ZoneFx_Zone2_DmaNormalPalettes();
s32 ZoneFx_Zone2_ReleaseSprites();

void ZoneFx_Zone2_Shutdown(u8 *p0) {
    ZoneFx_Zone2_StopVAlarms();
    ZoneFx_Zone2_DmaNormalPalettes(p0, 0);
    ZoneFx_Zone2_DmaNormalPalettes(p0, 1);
    ZoneFx_Zone2_ReleaseSprites(p0);
    s32 t0 = (*(u32 *)p0 | 0x80000000);
    *(u32 *)p0 = t0;
}
