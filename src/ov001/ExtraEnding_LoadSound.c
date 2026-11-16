#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 data_ov001_02317ea0[];
void *NNS_SndHeapCreate(void *p, u32 size);
void *NNS_SndHeapAlloc(void *h, u32 size, s32 a, s32 b, s32 c);
void *File_Load(void *p, void *q);
void func_0201d580(void *p, void *q);
void NNS_SndArcPlayerSetup(void *h);
void Sound_PlaySeq(s32 a, s32 b);

void ExtraEnding_LoadSound(u8 *p) {
    void *r;
    u8 *blk = Heap_Alloc(gMainHeap, 0x7d040, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x7d040);
        *(u32 *)blk = 0x7d040;
        *(u32 *)(blk + 0x7d03c) = 0x02467531;
        DC_StoreRange(blk, 0x7d040);
        blk += 0x20;
    }
    *(u8 **)(p + 0x10) = blk;
    *(void **)(p + 0xc) = NNS_SndHeapCreate(*(void **)(p + 0x10), 0x7d000);
    r = NNS_SndHeapAlloc(*(void **)(p + 0xc), 0x64000, 0, 0, 0);
    File_Load(data_ov001_02317ea0, r);
    func_0201d580(p + 0x14, r);
    NNS_SndArcPlayerSetup(*(void **)(p + 0xc));
    Sound_PlaySeq(0, 0);
}
