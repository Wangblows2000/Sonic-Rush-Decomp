#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u8 pad0[0x1c];
    u8 *buf;
} UnkWork_02067fe4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02067fe4 *work;
} UnkTask_02067fe4;

extern void *gMainHeap;
extern u32 gSystemFlags;
extern u16 gDemoInputMode;

static inline void Free_02067fe4(u8 *p) {
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

void DemoInput_Destructor(UnkTask_02067fe4 *t) {
    UnkWork_02067fe4 *w = t->work;

    if (w->buf != NULL)
        Free_02067fe4(w->buf);
    gDemoInputMode = 0;
    gSystemFlags &= ~0x40;
}
