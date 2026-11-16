#include <nitro/types.h>

typedef struct UnkStruct_02310bf4_n {
    u8 pad0[0xdc];
    struct UnkStruct_02310bf4_n *next;
} UnkStruct_02310bf4_n;

typedef struct UnkStruct_02310bf4 {
    u8 pad0[8];
    u8 *unk8;
    u8 *unkC;
    u8 pad10[0xec - 0x10];
    UnkStruct_02310bf4_n *head;
    u8 padF0[4];
    UnkStruct_02310bf4_n sentinel;
} UnkStruct_02310bf4;

extern void *gMainHeap;

void SpecialStageObjList_Free(UnkStruct_02310bf4 *g, UnkStruct_02310bf4_n *n);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

void SpecialStageObjList_Destroy(UnkStruct_02310bf4 *g) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    UnkStruct_02310bf4_n *n;
    u8 *p;
    u32 size;

    for (n = g->head; n != &g->sentinel; n = n->next)
        SpecialStageObjList_Free(g, n);
    p = g->unkC;
    if ((u8 *)OS_GetArenaLo(0) != p) {
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            fill1 = 0xcccccccc;
            MIi_CpuClear32(fill1, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    p = g->unk8;
    if ((u8 *)OS_GetArenaLo(0) != p) {
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    p = (u8 *)g;
    if ((u8 *)OS_GetArenaLo(0) != p) {
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            fill3 = 0xcccccccc;
            MIi_CpuClear32(fill3, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}
