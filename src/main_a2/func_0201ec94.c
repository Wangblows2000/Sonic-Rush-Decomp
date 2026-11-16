#include <nitro/types.h>

void OS_SleepThread(void *q);
void OS_LockMutex(void *m);
void OS_UnlockMutex(void *m);
void *func_0201f988(void *list);
void func_0201ed04(void *x);
void FreeCommandBuffer(void *x);

void func_0201ec94(u8 *p) {
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
            x = func_0201f988(list);
            if (x == NULL) {
                OS_UnlockMutex(mutex);
                break;
            }
            func_0201ed04(x);
            FreeCommandBuffer(x);
            OS_UnlockMutex(mutex);
        }
    }
}
