#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 *gExtraBossArchive;
extern s32 gZoneSoundData;
void ExtraBoss_ReleaseArchiveFiles24_25(void);
void ExtraBoss_ReleaseModelRes(void);
void ExtraBoss_ReleaseBgFiles(void);
void ExtraBoss_ReleaseArchiveFile27(void);
void ExtraBoss_ReleaseArchiveFile26(void);
void ExtraBoss_ReleaseObjResources(void);
void ExtraBoss_ReleaseArchiveFiles21_23(void);
void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);

void ExtraBoss_ReleaseAssets(void) {
    u8 *blk;
    ExtraBoss_ReleaseArchiveFiles24_25();
    ExtraBoss_ReleaseModelRes();
    ExtraBoss_ReleaseBgFiles();
    ExtraBoss_ReleaseArchiveFile27();
    ExtraBoss_ReleaseArchiveFile26();
    ExtraBoss_ReleaseObjResources();
    ExtraBoss_ReleaseArchiveFiles21_23();
    NNS_SndStopSoundAll();
    Sound_ResetHeap();
    gZoneSoundData = 0;
    blk = gExtraBossArchive;
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
    gExtraBossArchive = NULL;
}
