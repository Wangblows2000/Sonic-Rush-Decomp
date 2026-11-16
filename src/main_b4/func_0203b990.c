#include <nitro/types.h>

typedef struct {
    u32 data[0x11];
} UnkFile_0203b990;

extern void *gMainHeap;
extern u8 *data_022b89e4;

void func_0200be70(UnkFile_0203b990 *f);
BOOL func_0200bb20(UnkFile_0203b990 *f, u32 path);
void func_0200bad8(UnkFile_0203b990 *f);
u32 func_02017910(UnkFile_0203b990 *f);
BOOL func_020174a4(UnkFile_0203b990 *f, u8 *buf, u32 size);
BOOL func_02017148(u32 *path, u8 *buf);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline u8 *Alloc_0203b990(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, -0x20);
    if ((u8 *)func_02006abc(0) != p) {
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
    if ((u8 *)func_02006abc(0) != p) {
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

BOOL func_0203b990(u32 *path) {
    UnkFile_0203b990 *fp;
    u32 size;
    BOOL ok;
    u8 *p;
    UnkFile_0203b990 file;

    ok = FALSE;
    if (*path == 0) {
        fp = NULL;
    } else {
        func_0200be70(&file);
        if (!func_0200bb20(&file, *path))
            return FALSE;
        fp = &file;
    }
    size = func_02017910(fp);
    if (size != 0) {
        p = Alloc_0203b990((size + 0x43) & ~0x1f);
        data_022b89e4 = p;
        if (p != NULL) {
            if (func_020174a4(fp, p, size) && func_02017148(path, data_022b89e4))
                ok = TRUE;
            if (!ok)
                Free_0203b990(data_022b89e4);
        }
    }
    if (fp == &file)
        func_0200bad8(&file);
    return ok;
}
