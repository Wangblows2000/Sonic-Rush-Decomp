#include <nitro/types.h>

extern void *gSystemHeap;
extern u32 gSaveData;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 SaveData_CalcChecksum(void *p);
BOOL SaveData_ReadSlotPair(u8 *p);
BOOL SaveData_Compare(void *a, u8 *p);

static inline u8 *Alloc_02058e34(u32 size) {
    u8 *p = Heap_Alloc(gSystemHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void Free_02058e34(u8 *p) {
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
    Heap_Free(gSystemHeap, p);
}

BOOL SaveData_IsDirty(void) {
    u8 *p;

    p = Alloc_02058e34(0x340);
    if (p == (u8 *)OS_GetArenaLo(0))
        return FALSE;
    {
        vu32 zero = 0;
        MIi_CpuClear32(zero, p, 0x314);
    }
    gSaveData = SaveData_CalcChecksum(&gSaveData);
    if (SaveData_ReadSlotPair(p)) {
        Free_02058e34(p);
        return TRUE;
    }
    if (SaveData_Compare(&gSaveData, p)) {
        Free_02058e34(p);
        return FALSE;
    }
    Free_02058e34(p);
    return TRUE;
}
