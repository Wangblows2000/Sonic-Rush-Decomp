#include <nitro/types.h>
#include "game/GameSystem/task.h"

extern UnkTask *gTaskCurrent;
extern void *gSystemHeap;

void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);

static inline void UnkFree_020371c8(void *p) {
    u32 size;
    if (p == NULL) {
        return;
    }
    if (OS_GetArenaLo(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gSystemHeap, p);
}

void Task_Destroy(UnkTask *t) {
    if (OS_GetArenaLo(0) == t) {
        return;
    }
    if (t->flags & 1) {
        return;
    }
    if ((t->prio & 0x20) && gTaskCurrent != t) {
        return;
    }
    if (t->dtor != NULL) {
        t->dtor(t);
    }
    UnkFree_020371c8(t->work);
    t->work = NULL;
    t->flags |= 1;
}
