#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u32 frame; /* 0x0c */
    s16 fire;  /* 0x10 */
    s16 delay; /* 0x12 */
} UnkVAlarm_020070c0;

s32 OSi_CompareVCount(UnkVAlarm_020070c0 *alarm, u32 currentFrame, s32 currentVCount)
{
    s32 fDiff = (s32)(currentFrame - alarm->frame);
    s32 vDiff = currentVCount - alarm->fire;

    if (fDiff < 0 || (fDiff == 0 && vDiff < 0)) {
        return 0;
    }
    if (vDiff < 0) {
        vDiff += 263;
    }
    return (vDiff <= alarm->delay) ? 1 : 2;
}
