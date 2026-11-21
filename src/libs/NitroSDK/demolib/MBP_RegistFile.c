#include <nitro/types.h>

typedef struct {
    u32 data[0x11];
} UnkFile_0203b990;

extern void *gMainHeap;
extern u8 *sMbpFilebuf;

void FS_InitFile(UnkFile_0203b990 *f);
BOOL FS_OpenFile(UnkFile_0203b990 *f, u32 path);
void FS_CloseFile(UnkFile_0203b990 *f);
u32 MB_GetSegmentLength(UnkFile_0203b990 *f);
BOOL MB_ReadSegment(UnkFile_0203b990 *f, u8 *buf, u32 size);
BOOL MB_RegisterFile(u32 *path, u8 *buf);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline u8 *Alloc_0203b990(u32 sz) {
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

static inline void Free_0203b990(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

BOOL MBP_RegistFile(u32 *path) {
    UnkFile_0203b990 *fp;
    u32 size;
    BOOL ok;
    u8 *p;
    UnkFile_0203b990 file;

    ok = FALSE;
    if (*path == 0) {
        fp = NULL;
    } else {
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, *path))
            return FALSE;
        fp = &file;
    }
    size = MB_GetSegmentLength(fp);
    if (size != 0) {
        p = Alloc_0203b990((size + 0x43) & ~0x1f);
        sMbpFilebuf = p;
        if (p != NULL) {
            if (MB_ReadSegment(fp, p, size) && MB_RegisterFile(path, sMbpFilebuf))
                ok = TRUE;
            if (!ok)
                Free_0203b990(sMbpFilebuf);
        }
    }
    if (fp == &file)
        FS_CloseFile(&file);
    return ok;
}
