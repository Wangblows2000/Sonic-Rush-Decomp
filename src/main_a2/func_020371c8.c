#include <nitro/types.h>
#include "main_a2/task.h"

extern UnkTask *data_022b4574;
extern void *data_0208fe78;

void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkFree_020371c8(void *p) {
    u32 size;
    if (p == NULL) {
        return;
    }
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe78, p);
}

void func_020371c8(UnkTask *t) {
    if (func_02006abc(0) == t) {
        return;
    }
    if (t->flags & 1) {
        return;
    }
    if ((t->prio & 0x20) && data_022b4574 != t) {
        return;
    }
    if (t->dtor != NULL) {
        t->dtor(t);
    }
    UnkFree_020371c8(t->work);
    t->work = NULL;
    t->flags |= 1;
}
