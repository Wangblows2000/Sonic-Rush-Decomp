#include <nitro/types.h>

typedef struct UnkVAlarm_02007110 {
    void (*handler)(void *); /* 0x00 */
    void *arg;               /* 0x04 */
    u8 pad8[4];
    u32 frame;               /* 0x0c */
    s16 fire;                /* 0x10 */
    u8 pad12[0xa];
    u32 period;              /* 0x1c */
    u8 pad20[4];
    u32 canceled;            /* 0x24 */
} UnkVAlarm_02007110;

extern UnkVAlarm_02007110 *OSi_VAlarmQueue;
extern u32 OSi_VFrameCount;
extern u8 data_027e0000[];

u32 OS_DisableIrqMask(u32 mask);
u32 OS_ResetRequestIrqMask(u32 mask);
u32 OSi_GetVFrame(s32 vcount);
s32 OSi_CompareVCount(UnkVAlarm_02007110 *alarm, u32 frame, s32 vcount);
void OSi_SetNextVAlarm(UnkVAlarm_02007110 *alarm);
void OSi_DetachVAlarm(UnkVAlarm_02007110 *alarm);
void OSi_AppendVAlarm(UnkVAlarm_02007110 *alarm);

#define DISPSTAT (*(vu16 *)0x04000004)
#define VCOUNT (*(vu16 *)0x04000006)

static inline s32 func_02007110_GetVCount(void)
{
    return VCOUNT;
}

static inline s32 func_02007110_GetVCountEqVal(void)
{
    u16 d = DISPSTAT;
    return ((d >> 8) & 0xff) | ((d << 1) & 0x100);
}

void OSi_VAlarmHandler(void *arg)
{
    UnkVAlarm_02007110 *alarm;
    s32 currentVCount;
    u32 currentFrame;

    (void)OS_DisableIrqMask(4);
    DISPSTAT &= ~0x20;
    *(vu32 *)((u32)data_027e0000 + 0x3ff8) |= 4;
    (void)OSi_GetVFrame(func_02007110_GetVCountEqVal() - 1);
    while ((alarm = OSi_VAlarmQueue) != NULL) {
        currentVCount = VCOUNT;
        currentFrame = OSi_GetVFrame(currentVCount);
        switch (OSi_CompareVCount(alarm, currentFrame, currentVCount)) {
        case 0:
            OSi_SetNextVAlarm(alarm);
            if (alarm->fire != func_02007110_GetVCount() || alarm->frame != currentFrame) {
                return;
            }
            (void)OS_DisableIrqMask(4);
            DISPSTAT &= ~0x20;
            (void)OS_ResetRequestIrqMask(4);
        case 1: {
            void (*handler)(void *) = alarm->handler;
            OSi_DetachVAlarm(alarm);
            alarm->handler = NULL;
            if (handler) {
                handler(alarm->arg);
            }
            if (alarm->period && !alarm->canceled) {
                alarm->handler = handler;
                alarm->frame = OSi_VFrameCount + 1;
                OSi_AppendVAlarm(alarm);
            }
            break;
        }
        case 2:
            OSi_DetachVAlarm(alarm);
            OSi_AppendVAlarm(alarm);
            alarm->frame = OSi_VFrameCount + 1;
            break;
        }
    }
}
