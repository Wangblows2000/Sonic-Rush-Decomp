/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_CancelVAlarm();
void ZoneFx_Zone2_VAlarmTintedPalettes();
s32 OS_SetVAlarm();
void ZoneFx_Zone2_VAlarmNormalPalettes();

void ZoneFx_Zone2_SetVAlarms(s32 p0, s32 p1) {
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)));
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)));
    OS_SetVAlarm(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)), *(u8 *)((u8 *)((s32)p0 + (s32)p1) + 0xa), 5, (u32)ZoneFx_Zone2_VAlarmTintedPalettes, p1);
    OS_SetVAlarm(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)), 0xc8, 5, (u32)ZoneFx_Zone2_VAlarmNormalPalettes, p1);
}
