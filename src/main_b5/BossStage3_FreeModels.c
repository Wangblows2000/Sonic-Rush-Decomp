#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

extern void *gMainHeap;
extern u8 *data_ov000_02369b18[];

static inline void Free_0204a01c(u8 *p) {
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

void BossStage3_FreeModels(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (data_ov000_02369b18[i] != NULL) {
            Free_0204a01c(data_ov000_02369b18[i]);
            data_ov000_02369b18[i] = NULL;
        }
    }
}
