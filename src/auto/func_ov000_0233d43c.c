/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_CancelVAlarm();
void func_01fff7c4();
s32 OS_SetVAlarm();
void func_01fff760();

void func_ov000_0233d43c(s32 p0, s32 p1) {
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)));
    OS_CancelVAlarm(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)));
    OS_SetVAlarm(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)), *(u8 *)((u8 *)((s32)p0 + (s32)p1) + 0xa), 5, (u32)func_01fff7c4, p1);
    OS_SetVAlarm(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)), 0xc8, 5, (u32)func_01fff760, p1);
}
