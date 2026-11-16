#include <nitro/types.h>

typedef struct UnkVAlarm {
    void (*handler)(void *); /* 0x00 */
    void *arg;               /* 0x04 */
    u8 pad8[4];
    u32 frame;               /* 0x0c */
    s16 fire;                /* 0x10 */
    s16 delay;               /* 0x12 */
    struct UnkVAlarm *prev;  /* 0x14 */
    struct UnkVAlarm *next;  /* 0x18 */
    u32 period;              /* 0x1c */
    u8 pad20[4];
    u32 canceled;            /* 0x24 */
} UnkVAlarm;

extern UnkVAlarm *data_0207d394;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void OS_Terminate(void);
u32 func_02007074(s32 vcount);
void func_020074e8(UnkVAlarm *alarm);

void func_020073b8(UnkVAlarm *alarm, s16 count, s16 delay, void (*handler)(void *), void *arg)
{
    u32 enabled;
    s32 currentVCount;
    u32 frame;

    enabled = OS_DisableInterrupts();
    if (!alarm || alarm->handler) {
        OS_Terminate();
    }
    currentVCount = *(vu16 *)0x04000006;
    frame = func_02007074(currentVCount);
    alarm->period = 0;
    alarm->fire = count;
    if (count <= currentVCount) {
        frame++;
    }
    alarm->frame = frame;
    alarm->delay = delay;
    alarm->handler = handler;
    alarm->arg = arg;
    alarm->canceled = 0;
    func_020074e8(alarm);
    (void)OS_RestoreInterrupts(enabled);
}
