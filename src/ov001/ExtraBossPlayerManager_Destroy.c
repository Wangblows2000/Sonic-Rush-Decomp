#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
typedef struct { u8 pad[0xbc]; void *objs[0x2e]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *gExtraBossPlayerManager;
void ExtraBossObj_Destroy(void *p);

void ExtraBossPlayerManager_Destroy(void) {
    u8 *blk;
    s32 i;
    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->objs[i] != NULL) {
            ExtraBossObj_Destroy(gExtraBossPlayerManager->objs[i]);
        }
    }
    blk = (u8 *)gExtraBossPlayerManager;
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
    gExtraBossPlayerManager = NULL;
}
