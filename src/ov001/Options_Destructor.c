#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 gPad[];
void Touch_Disable(void);
void Pad_Reset(void *p, s32 a, s32 b);
void Options_ReleaseAssets(void *p);
void Options_ResetDisplay(void);

#pragma thumb on
void Options_Destructor(u8 *t) {
    u8 *p = *(u8 **)(t + 0x10);
    u8 *blk = *(u8 **)(p + 0x1e04);
    if (blk != NULL) {
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
        *(void **)(p + 0x1e04) = NULL;
    }
    Touch_Disable();
    Pad_Reset(gPad, 0, 0);
    Options_ReleaseAssets(p);
    Options_ResetDisplay();
}
#pragma thumb reset
