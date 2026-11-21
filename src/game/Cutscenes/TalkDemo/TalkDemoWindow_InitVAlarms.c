/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 OS_InitVAlarm();
s32 OS_CreateVAlarm();

void TalkDemoWindow_InitVAlarms(s32 p0) {
    OS_InitVAlarm();
    OS_CreateVAlarm(((u8 *)p0 + 0x4c));
    OS_CreateVAlarm(((u8 *)p0 + 0x74));
    OS_CreateVAlarm(((u8 *)p0 + 0x9c));
    OS_CreateVAlarm(((u8 *)p0 + 0xc4));
    OS_CreateVAlarm(((u8 *)p0 + 0x24));
}

#pragma thumb reset
