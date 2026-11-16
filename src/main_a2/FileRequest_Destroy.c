#include <nitro/types.h>

typedef struct UnkNode_02033e3c {
    struct UnkNode_02033e3c *next;
    struct UnkNode_02033e3c *prev;
    u8 unk8[4];
    u32 flags;
    u8 unk10[8];
    u8 unk18[0x44];
    void *mem;
} UnkNode_02033e3c;

extern u32 gFileRequestUsed;
extern UnkNode_02033e3c *gFileRequestFreeList[];
extern void *gMainHeap;

void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);
void func_0200bad8(void *p);

static inline void UnkFree_02033e3c(void **pp) {
    u32 size;
    void *p = *pp;
    if (p == NULL || p == (void *)-1) {
        return;
    }
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    *pp = NULL;
}

void FileRequest_Destroy(UnkNode_02033e3c *n) {
    n->next->prev = n->prev;
    n->prev->next = n->next;
    gFileRequestFreeList[--gFileRequestUsed] = n;
    if ((n->flags & 1) && n->mem != NULL) {
        UnkFree_02033e3c(&n->mem);
    }
    if (n->flags & 2) {
        func_0200bad8(n->unk18);
    }
}
