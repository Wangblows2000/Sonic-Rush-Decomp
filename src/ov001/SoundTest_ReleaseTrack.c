#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void NNS_SndCaptureStopSampling(void *p);
void NNS_SndStopSoundAll(void);
void NNS_SndStrmHandleRelease(void *p);
void Sound_ResetHeap(void);

#pragma thumb on
void SoundTest_ReleaseTrack(u8 *p) {
    u8 *blk;
    if (*(void **)p != NULL) {
        NNS_SndCaptureStopSampling(*(void **)p);
        *(void **)p = NULL;
        NNS_SndStopSoundAll();
        if (*(void **)(p + 4) != NULL) {
            NNS_SndStrmHandleRelease(p + 0x1a4);
            *(void **)(p + 4) = NULL;
        }
        Sound_ResetHeap();
    }
    blk = *(u8 **)(p + 0x1b0);
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
        *(void **)(p + 0x1b0) = NULL;
    }
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xc) = 0;
    *(s32 *)(p + 0x1a8) = 0;
    *(u16 *)(p + 0x1ac) = 0;
    *(u16 *)(p + 0x1ae) = 0;
}
#pragma thumb reset
