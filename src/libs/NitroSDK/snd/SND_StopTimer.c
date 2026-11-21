#include <nitro/types.h>

extern void SNDi_IncAlarmId(s32 alarmNo);
extern void PushCommand_impl(u32 command, u32 a, u32 b, u32 c, u32 d);

void SND_StopTimer(u32 a, u32 b, u32 alarmMask, u32 d)
{
    s32 i = 0;
    u32 bitmask = alarmMask;
    while (i < 8 && bitmask != 0) {
        if (bitmask & 1) {
            SNDi_IncAlarmId(i);
        }
        i++;
        bitmask >>= 1;
    }
    PushCommand_impl(0xd, a, b, alarmMask, d);
}
