#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u32 data[0x12];
} UnkFile_0203879c;

typedef struct {
    u32 ofs;
    u32 size;
} UnkInfo_0203879c;

extern void *gMainHeap;

void FS_InitFile(UnkFile_0203879c *f);
BOOL FS_OpenFile(UnkFile_0203879c *f, const char *path);
void File_ReadArchiveEntryInfo(UnkFile_0203879c *f, const char *name, UnkInfo_0203879c *info);
void FS_SeekFile(UnkFile_0203879c *f, u32 ofs, s32 whence);
void FS_ReadFile(UnkFile_0203879c *f, void *buf, u32 size);
void FS_CloseFile(UnkFile_0203879c *f);

static inline u8 *Alloc_0203879ca(u32 sz) {
    u32 size = sz;
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

static inline u8 *Alloc_0203879cb(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, -0x20);
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

u8 *File_LoadArchiveEntry(const char *path, const char *name, u8 *p) {
    UnkInfo_0203879c info;
    UnkFile_0203879c f;

    FS_InitFile(&f);
    FS_OpenFile(&f, path);
    File_ReadArchiveEntryInfo(&f, name, &info);
    if (p == NULL)
        p = Alloc_0203879ca((info.size + 0x43) & ~0x1f);
    else if (p == (u8 *)-1)
        p = Alloc_0203879cb((info.size + 0x43) & ~0x1f);
    FS_SeekFile(&f, info.ofs, 0);
    FS_ReadFile(&f, p, info.size);
    FS_CloseFile(&f);
    DC_StoreRange(p, info.size);
    return p;
}
