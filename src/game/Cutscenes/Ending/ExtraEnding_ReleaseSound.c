#include <nitro/types.h>

typedef struct { u8 pad[0xc]; void *heap; u8 *mem; } UnkStruct_022c61c8;
extern void *gMainHeap;
void NNS_SndStopSoundAll(void);
void NNS_SndHeapClear(void *h);
void NNS_SndHeapDestroy(void *h);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Sound_ResetHeap(void);

void ExtraEnding_ReleaseSound(UnkStruct_022c61c8 *p) {
    u8 *blk;
    NNS_SndStopSoundAll();
    NNS_SndHeapClear(p->heap);
    NNS_SndHeapDestroy(p->heap);
    blk = p->mem;
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
    Sound_ResetHeap();
}
