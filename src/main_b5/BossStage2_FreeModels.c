#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

extern void *gMainHeap;
extern u8 *gBossStage2Models[];

static inline void Free_0204a0d0(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
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

void BossStage2_FreeModels(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (gBossStage2Models[i] != NULL) {
            Free_0204a0d0(gBossStage2Models[i]);
            gBossStage2Models[i] = NULL;
        }
    }
}
