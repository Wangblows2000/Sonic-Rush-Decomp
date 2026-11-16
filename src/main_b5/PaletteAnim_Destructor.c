#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u8 *buf;
    u16 idx;
    u8 pad6[2];
    u16 flags;
} UnkWork_02044040;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02044040 *work;
} UnkTask_02044040;

extern void *gMainHeap;
extern u16 gPaletteAnimRefCount[];
extern u32 gPaletteAnimTasks[];

static inline void Free_02044040(u8 *p) {
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void PaletteAnim_Destructor(UnkTask_02044040 *t) {
    UnkWork_02044040 *w = t->work;

    gPaletteAnimRefCount[w->idx] = 0;
    gPaletteAnimTasks[w->idx] = 0;
    if (!(w->flags & 1))
        return;
    Free_02044040(w->buf);
}
