#include <nitro/types.h>

extern void *gMainHeap;
extern u8 data_ov001_0231d9b0[];
extern u8 data_ov001_0231d9cc[];
extern u8 *gExtraBossArchive;
extern void *gSoundHeap;
extern void *gZoneSoundData;

u8 *File_Load(void *p, void *dst);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 ExtraBoss_GetCompressionType(u32 *p);
u32 ExtraBoss_GetUncompressedSize(u32 *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
u32 *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
void ExtraBoss_Unk_ov001_023029d4(void);
void ExtraBoss_LoadBossModel(void);
void ExtraBoss_Unk_ov001_02305368(void);
void ExtraBoss_Unk_ov001_02304518(void);
void ExtraBoss_Unk_ov001_022fdab4(void);
void ExtraBoss_Unk_ov001_022fd564(void);
void ExtraBoss_Unk_ov001_023065f8(void);
void *NNS_SndHeapGetFreeSize(void *p);
void *NNS_SndHeapAlloc(void *s, u32 size, u32 a, u32 b, u32 c);
void Sound_LoadSoundArchiveFromMemory(void *a);

void ExtraBoss_LoadAssets(void) {
    vu32 fill1;
    vu32 fill2;
    u8 *res;
    u32 size;
    u8 *blk;

    res = File_Load(data_ov001_0231d9b0, (void *)-1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    gExtraBossArchive = blk;
    switch (ExtraBoss_GetCompressionType((u32 *)res)) {
    case 0x10:
        MI_UncompressLZ16(res, blk);
        break;
    case 0x20:
        MI_UncompressHuffman(res, blk);
        break;
    case 0x30:
        MI_UncompressRL16(res, blk);
        break;
    default:
        Mem_Copy(Compress_GetData(res), blk, ExtraBoss_GetUncompressedSize((u32 *)res));
        break;
    }
    DC_StoreRange(blk, ExtraBoss_GetUncompressedSize((u32 *)res));
    if ((u8 *)OS_GetArenaLo(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, res, size);
            DC_StoreRange(res, size);
        }
    }
    Heap_Free(gMainHeap, res);
    ExtraBoss_Unk_ov001_023029d4();
    ExtraBoss_LoadBossModel();
    ExtraBoss_Unk_ov001_02305368();
    ExtraBoss_Unk_ov001_02304518();
    ExtraBoss_Unk_ov001_022fdab4();
    ExtraBoss_Unk_ov001_022fd564();
    ExtraBoss_Unk_ov001_023065f8();
    gZoneSoundData = NNS_SndHeapGetFreeSize(gSoundHeap);
    gZoneSoundData = NNS_SndHeapAlloc(gSoundHeap, 0x60400, 0, 0, 0);
    File_Load(data_ov001_0231d9cc, gZoneSoundData);
    Sound_LoadSoundArchiveFromMemory(gZoneSoundData);
}
