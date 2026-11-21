#ifndef MAIN_SDK2_OSMSG_H
#define MAIN_SDK2_OSMSG_H
#include <nitro/types.h>

typedef struct {
    u16 queueSend;    /* 0x00 */
    u16 queueReceive; /* 0x02 */
    void **msgArray;  /* 0x04 */
    s32 msgCount;     /* 0x08 */
    s32 firstIndex;   /* 0x0c */
    s32 usedCount;    /* 0x10 */
} UnkOSMessageQueue;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_SleepThread(u16 *queue);
void OS_WakeupThread(u16 *queue);
#endif
