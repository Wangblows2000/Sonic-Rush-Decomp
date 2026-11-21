/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 OSi_UseVAlarm[];
extern u8 OSi_VAlarmQueue[];
s32 OS_DisableIrqMask();
extern u8 OSi_VFrameCount[];
extern u8 OSi_PreviousVCount[];

void OS_InitVAlarm(void) {
    if (*(u16 *)OSi_UseVAlarm != 0) {
        return;
    }
    *(u16 *)OSi_UseVAlarm = 1;
    *(u32 *)OSi_VAlarmQueue = 0;
    *(u32 *)((u8 *)OSi_VAlarmQueue + 0x4) = 0;
    OS_DisableIrqMask(4, OSi_VAlarmQueue, OSi_UseVAlarm, 0);
    *(u32 *)OSi_VFrameCount = 0;
    *(u32 *)OSi_PreviousVCount = 0;
}
