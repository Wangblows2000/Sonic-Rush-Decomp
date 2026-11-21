#include <nitro/types.h>

void OS_SleepThread(void *q);
void OS_LockMutex(void *m);
void OS_UnlockMutex(void *m);
void *ReadCommandBuffer(void *list);
void MakeWaveData(void *x);
void FreeCommandBuffer(void *x);

void StrmThread(u8 *p) {
    void *queue;
    void *mutex;
    void *list;
    void *x;
    mutex = p + 0x498;
    list = p + 0x4AC;
    queue = p + 0x494;
    for (;;) {
        OS_SleepThread(queue);
        for (;;) {
            OS_LockMutex(mutex);
            x = ReadCommandBuffer(list);
            if (x == NULL) {
                OS_UnlockMutex(mutex);
                break;
            }
            MakeWaveData(x);
            FreeCommandBuffer(x);
            OS_UnlockMutex(mutex);
        }
    }
}
