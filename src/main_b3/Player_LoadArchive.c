#include <nitro/types.h>

extern u8 data_0207a208[];
extern void *gMainHeap;
extern u8 *data_ov000_0236ac28;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void func_ov000_0234dee0(void);
void Player_LoadAssets(void);
u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 Player_GetArchiveCompressionType(void *p);
u32 Player_GetArchiveUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void NaviBg_Load(s32 a, s32 b);
void NaviB_Load(void);

static inline u8 *Alloc_0204e934(u32 size) {
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
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

static inline void Free_0204e934(u8 *blk) {
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
}

void Player_LoadArchive(void) {
    u8 *f;
    u8 *p;
    u32 size;

    func_ov000_0234dee0();
    f = File_Load(data_0207a208, -1);
    size = ((*(u32 *)f >> 8) + 0x43) & ~0x1f;
    p = Alloc_0204e934(size);
    data_ov000_0236ac28 = p;
    switch (Player_GetArchiveCompressionType(f)) {
    case 0x10:
        MI_UncompressLZ16(f, p);
        break;
    case 0x20:
        MI_UncompressHuffman(f, p);
        break;
    case 0x30:
        MI_UncompressRL16(f, p);
        break;
    default:
        Mem_Copy(Compress_GetData(f), p, Player_GetArchiveUncompressedSize(f));
        break;
    }
    DC_StoreRange(p, Player_GetArchiveUncompressedSize(f));
    Free_0204e934(f);
    Player_LoadAssets();
    NaviBg_Load(1, data_022c4560.unk0);
    NaviB_Load();
}
