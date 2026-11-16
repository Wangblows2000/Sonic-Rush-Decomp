/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gZoneFxWork[];
s32 ZoneFx_Zone2_DmaTintedPalettes();
s32 OS_SetVAlarm();

void ZoneFx_Zone2_VAlarmTintedPalettes(s32 p0) {
    s32 t0 = *(u32 *)gZoneFxWork;
    if (t0 == 0) {
        return;
    }
    ZoneFx_Zone2_DmaTintedPalettes(t0, p0);
    OS_SetVAlarm(((s32)p0 * 0x28 + (t0 + 0x5c)), *(u8 *)((u8 *)(t0 + (s32)p0) + 0xa), 5, (u32)ZoneFx_Zone2_VAlarmTintedPalettes, p0);
}
