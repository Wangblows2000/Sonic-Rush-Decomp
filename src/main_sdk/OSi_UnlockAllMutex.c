#include <nitro/types.h>

typedef struct { u8 pad[4]; u32 x4; u32 x8; } UnkStruct_020065d8_T;
typedef struct { u8 pad[0x7c]; UnkStruct_020065d8_T *head; } UnkStruct_020065d8;
UnkStruct_020065d8_T *OSi_DequeueHead(UnkStruct_020065d8 *q);
void OS_WakeupThread(UnkStruct_020065d8_T *t);
void OSi_UnlockAllMutex(UnkStruct_020065d8 *q) {
    UnkStruct_020065d8_T *t;
    if (q->head == 0) return;
    do { t = OSi_DequeueHead(q); t->x8 = 0; t->x4 = 0; OS_WakeupThread(t); } while (q->head != 0);
}
