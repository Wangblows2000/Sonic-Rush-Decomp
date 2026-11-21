/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_CancelVAlarm();

void ZoneFx_Zone2_CancelVAlarms(s32 p0, s32 p1) {
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0xc) + (p1 * 0x28)), p1, 0x28);
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0x5c) + (p1 * 0x28)));
}
