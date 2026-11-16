#ifndef MAIN_A2_FILELOAD_H
#define MAIN_A2_FILELOAD_H

#include <nitro/types.h>

typedef struct UnkFile {
    u8 unk0[0x20];
    u32 top;
    u32 bottom;
    u8 unk28[0x1c];
} UnkFile;

typedef struct UnkFileId {
    void *arc;
    u32 id;
} UnkFileId;

typedef struct UnkLoadNode {
    struct UnkLoadNode *prev;
    struct UnkLoadNode *next;
    s32 status;
    u32 flags;
    UnkFileId fileId;
    UnkFile file;
    void *mem;
    u32 size;
} UnkLoadNode;

extern void *gMainHeap;

void *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkDbgAlloc(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = Heap_Alloc(gMainHeap, sz, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

#endif
