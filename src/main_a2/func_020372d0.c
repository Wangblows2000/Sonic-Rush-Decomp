#include <nitro/types.h>
#include "main_a2/task.h"

extern UnkTask *data_022b457c;
extern UnkTask *data_022b4580;
extern UnkTask *data_022b4584;
extern UnkTask *data_022b4588;
extern void *data_0208fe78;

UnkTask *func_02036e9c(void);
void *func_02006abc(u32 arg);
void *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkAlloc_020372d0(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = func_02034a40(data_0208fe78, sz, align);
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

UnkTask *func_020372d0(void (*func)(void), void (*dtor)(UnkTask *t), u16 prio, u16 id, u16 unk1a, u32 workSize) {
    UnkTask *t = func_02036e9c();
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
            pos = data_022b4580;
        } else {
            pos = data_022b457c->next;
            while (id >= pos->id) {
                pos = pos->next;
            }
        }
    } else {
        if (id == 0xffff) {
            pos = data_022b4588;
        } else {
            pos = data_022b4584->next;
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
