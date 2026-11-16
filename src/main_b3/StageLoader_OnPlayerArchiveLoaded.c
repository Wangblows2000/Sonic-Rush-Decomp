#include <nitro/types.h>

extern void *gMainHeap;
extern u8 *data_ov000_0236ac28;
typedef struct { u8 pad0[0x5c]; u8 *file; } UnkWork_0204ad2c;

void Player_LoadAssets(void);
u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 Compress_GetType(void *p);
u32 Compress_GetUncompressedSize(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);

static inline void Free_0204ad2c(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
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

void StageLoader_OnPlayerArchiveLoaded(UnkWork_0204ad2c *w) {
    u8 *f;
    u8 *p;
    u32 size;

    size = ((*(u32 *)w->file >> 8) + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, -0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    data_ov000_0236ac28 = p;
    f = w->file;
    switch (Compress_GetType(f)) {
    case 0x10:
        func_02008648(f, p);
        break;
    case 0x20:
        func_02008720(f, p);
        break;
    case 0x30:
        func_020087f8(f, p);
        break;
    default:
        Mem_Copy(Compress_GetData(f), p, Compress_GetUncompressedSize(f));
        break;
    }
    DC_StoreRange(p, Compress_GetUncompressedSize(f));
    Free_0204ad2c(w->file);
    w->file = NULL;
    Player_LoadAssets();
}
