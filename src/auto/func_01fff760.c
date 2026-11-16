/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gZoneFxWork[];
s32 ZoneFx_Zone2_DmaNormalPalettes();
s32 OS_SetVAlarm();

void func_01fff760(s32 p0) {
    s32 t0 = *(u32 *)gZoneFxWork;
    if (t0 == 0) {
        return;
    }
    ZoneFx_Zone2_DmaNormalPalettes(t0, p0);
    OS_SetVAlarm(((s32)p0 * 0x28 + (t0 + 0xc)), 0xc8, 5, (u32)func_01fff760, p0);
}
