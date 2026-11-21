#include <nitro/types.h>

void OS_CreateThread(void *thread, void (*func)(u8 *), void *arg, void *stack, u32 size, u32 prio);
void StrmThread(u8 *p);
void NNS_FndInitList(void *list, u16 offset);
void OS_InitMutex(void *m);
void OS_WakeupThreadDirect(void *thread);

void CreateThread(u8 *p, u32 prio) {
    OS_CreateThread(p, StrmThread, p, p + 0x494, 0x400, prio);
    NNS_FndInitList(p + 0x4AC, 0);
    OS_InitMutex(p + 0x498);
    *(u16 *)(p + 0x494) = 0;
    OS_WakeupThreadDirect(p);
}
