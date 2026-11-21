#include <nitro/types.h>

typedef struct {
    u8 unk0[0x20];
    u32 top;
    u32 bottom;
    u8 unk28[0x20];
} UnkFile_02033fbc;

extern void *gMainHeap;

void FS_InitFile(UnkFile_02033fbc *f);
BOOL FS_OpenFile(UnkFile_02033fbc *f, const char *path);
s32 FS_ReadFile(UnkFile_02033fbc *f, void *dst, s32 len);
BOOL FS_CloseFile(UnkFile_02033fbc *f);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkAlloc_02033fbc(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = Heap_Alloc(gMainHeap, sz, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

void *File_Load(const char *path, void *dst) {
    UnkFile_02033fbc file;
    u32 size;
    FS_InitFile(&file);
    FS_OpenFile(&file, path);
    size = file.bottom - file.top;
    if (dst == NULL) {
        dst = UnkAlloc_02033fbc(size, 0x20);
    } else if (dst == (void *)-1) {
        dst = UnkAlloc_02033fbc(size, -0x20);
    }
    FS_ReadFile(&file, dst, size);
    FS_CloseFile(&file);
    DC_StoreRange(dst, size);
    return dst;
}
