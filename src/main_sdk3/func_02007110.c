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

extern UnkVAlarm_02007110 *data_0207d394;
extern u32 data_0207d390;
extern u8 data_027e0000[];

u32 func_02005348(u32 mask);
u32 OS_ResetRequestIrqMask(u32 mask);
u32 func_02007074(s32 vcount);
s32 func_020070c0(UnkVAlarm_02007110 *alarm, u32 frame, s32 vcount);
void func_02007374(UnkVAlarm_02007110 *alarm);
void func_02007464(UnkVAlarm_02007110 *alarm);
void func_0200749c(UnkVAlarm_02007110 *alarm);

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

void func_02007110(void *arg)
{
    UnkVAlarm_02007110 *alarm;
    s32 currentVCount;
    u32 currentFrame;

    (void)func_02005348(4);
    DISPSTAT &= ~0x20;
    *(vu32 *)((u32)data_027e0000 + 0x3ff8) |= 4;
    (void)func_02007074(func_02007110_GetVCountEqVal() - 1);
    while ((alarm = data_0207d394) != NULL) {
        currentVCount = VCOUNT;
        currentFrame = func_02007074(currentVCount);
        switch (func_020070c0(alarm, currentFrame, currentVCount)) {
        case 0:
            func_02007374(alarm);
            if (alarm->fire != func_02007110_GetVCount() || alarm->frame != currentFrame) {
                return;
            }
            (void)func_02005348(4);
            DISPSTAT &= ~0x20;
            (void)OS_ResetRequestIrqMask(4);
        case 1: {
            void (*handler)(void *) = alarm->handler;
            func_02007464(alarm);
            alarm->handler = NULL;
            if (handler) {
                handler(alarm->arg);
            }
            if (alarm->period && !alarm->canceled) {
                alarm->handler = handler;
                alarm->frame = data_0207d390 + 1;
                func_0200749c(alarm);
            }
            break;
        }
        case 2:
            func_02007464(alarm);
            func_0200749c(alarm);
            alarm->frame = data_0207d390 + 1;
            break;
        }
    }
}
