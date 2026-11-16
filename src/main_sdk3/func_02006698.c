#include <nitro/types.h>

typedef struct UnkThread_02006698 {
    u8 pad0[0x78];
    struct UnkMutex_02006698 *mutex; /* 0x78 */
} UnkThread_02006698;

typedef struct UnkMutex_02006698 {
    u32 queue;                     /* 0x00 */
    UnkThread_02006698 *thread;    /* 0x04 */
    s32 count;                     /* 0x08 */
} UnkMutex_02006698;

typedef struct {
    u8 pad0[8];
    UnkThread_02006698 *current;   /* 0x08 */
} UnkThreadInfo_02006698;

extern UnkThreadInfo_02006698 data_0207d090;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_020065b4(UnkThread_02006698 *thread, UnkMutex_02006698 *mutex);
void func_02005c48(void *queue);

void func_02006698(UnkMutex_02006698 *mutex)
{
    u32 enabled = OS_DisableInterrupts();
    UnkThread_02006698 *currentThread = data_0207d090.current;
    UnkThread_02006698 *ownerThread;

    for (;;) {
        ownerThread = ((volatile UnkMutex_02006698 *)mutex)->thread;
        if (ownerThread == NULL) {
            mutex->thread = currentThread;
            mutex->count++;
            func_020065b4(currentThread, mutex);
            break;
        } else if (ownerThread == currentThread) {
            mutex->count++;
            break;
        } else {
            currentThread->mutex = mutex;
            func_02005c48(&mutex->queue);
            currentThread->mutex = NULL;
        }
    }
    (void)OS_RestoreInterrupts(enabled);
}
