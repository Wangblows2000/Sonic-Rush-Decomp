#include <nitro/types.h>

typedef struct { u8 pad0[0x5c]; u8 *file; } UnkWork_0204aaac;
extern void *gMainHeap;
extern u8 *gZoneActArchive;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
s32 Compress_GetType(void *p);
u32 Compress_GetUncompressedSize(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);

void StageLoader_OnZoneActArchiveLoaded(UnkWork_0204aaac *w) {
    u8 *f;
    u8 *p;
    u32 size;

    size = ((*(u32 *)w->file >> 8) + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    gZoneActArchive = p;
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
}
