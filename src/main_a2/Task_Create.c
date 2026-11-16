#include <nitro/types.h>
#include "main_a2/task.h"

extern UnkTask *gTaskListMainHead;
extern UnkTask *gTaskListMainTail;
extern UnkTask *gTaskListVBlankHead;
extern UnkTask *gTaskListVBlankTail;
extern void *gSystemHeap;

UnkTask *Task_AllocNode(void);
void *func_02006abc(u32 arg);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkAlloc_020372d0(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = Heap_Alloc(gSystemHeap, sz, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

UnkTask *Task_Create(void (*func)(void), void (*dtor)(UnkTask *t), u16 prio, u16 id, u16 unk1a, u32 workSize) {
    UnkTask *t = Task_AllocNode();
    UnkTask *pos;
    if (func_02006abc(0) == t) {
        return func_02006abc(0);
    }
    t->func = func;
    t->dtor = dtor;
    t->flags = 0;
    t->prio = prio;
    t->id = id;
    t->unk1a = unk1a;
    t->work = NULL;
    if (workSize != 0) {
        t->work = UnkAlloc_020372d0(workSize, 0x20);
    }
    if (id <= 0xefff) {
        if (id == 0xefff) {
            pos = gTaskListMainTail;
        } else {
            pos = gTaskListMainHead->next;
            while (id >= pos->id) {
                pos = pos->next;
            }
        }
    } else {
        if (id == 0xffff) {
            pos = gTaskListVBlankTail;
        } else {
            pos = gTaskListVBlankHead->next;
            while (id >= pos->id) {
                pos = pos->next;
            }
        }
    }
    t->prev = pos->prev;
    t->next = pos;
    t->prev->next = t;
    pos->prev = t;
    return t;
}
